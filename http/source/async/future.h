/* future.h
   Purpose: retrieval of values computed asynchronously */

#pragma once

#include "event.h"
#include <cstring>
#include <optional>
#include <type_traits>
#include <variant>

namespace http
{

enum either_side : unsigned char 
{
    kEitherLeft, 
    kEitherRight 
};

template<class _Ty, class _Xy>
struct either
{
    inline either(const _Ty& left)
    {
        this->EmplaceSide(left);
    }

    inline either(const _Xy& right)
    {
        this->EmplaceSide(right);
    }

    inline either()
    { }

    inline ~either() {}

    inline std::optional<_Ty> GetLeft() const
    {
        if (Type != kEitherLeft) 
            return std::nullopt;
        
        return std::get<0>(LeftRight);
    }

    inline std::optional<_Xy> GetRight() const
    {
        if (Type != kEitherRight) 
            return std::nullopt;
        
        return std::get<1>(LeftRight);
    }

    /* Default-constructs left or right 
       and moves val to its pos */
    template<class _Rl>
    constexpr void EmplaceSide(_Rl& val)
    {
        using nocvrl_t = std::remove_cv_t<_Rl>;
        using nocvty_t = std::remove_cv_t<_Ty>;
        using nocvxy_t = std::remove_cv_t<_Xy>;

        static_assert(std::is_same_v<nocvrl_t, nocvty_t> 
                   || std::is_same_v<nocvrl_t, nocvxy_t>);
        if constexpr (std::is_same_v<nocvrl_t, nocvty_t>)
        {
            Type = kEitherLeft;
        }
        else if constexpr (std::is_same_v<nocvrl_t, nocvxy_t>)
        {
            Type = kEitherRight;
        }

        LeftRight = val;
    }

    either_side Type;
    std::variant<_Ty, _Xy> LeftRight;
};

template<class _Ty, class _Ey = int>
class CFuture
{
public:
    /* Maybe add mutexes. */
    constexpr void SetPositiveResult(_Ty& result)
    {
        m_Result.EmplaceSide(result);
        m_Event.Set();
    }

    constexpr void SetNegativeResult(_Ey& result)
    {
        m_Result.EmplaceSide(result);
        m_Event.Set();
    }

    constexpr either<_Ty, _Ey> GetResult()
    {
        m_Event.Wait();
        return m_Result;
    }

private:
    either<_Ty, _Ey> m_Result;
    CEvent m_Event;
};

}

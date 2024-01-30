/* event.h
   Purpose: Implementation of a event semaphore */

#pragma once

#include <pthread.h>

namespace http
{

class CEvent
{
public:
    CEvent(const CEvent& rhs)
        : m_Flag(rhs.m_Flag),
          m_Condition(rhs.m_Condition),
          m_Mutex(rhs.m_Mutex)
    {

    }

    inline CEvent()
        : m_Flag(false)
    {
        pthread_cond_init(&m_Condition, nullptr);
        pthread_mutex_init(&m_Mutex, nullptr);
    }

    inline ~CEvent()
    {
        pthread_cond_destroy(&m_Condition);
        pthread_mutex_destroy(&m_Mutex);
    }

    inline void Set()
    {
        if (m_Flag) return;

        pthread_mutex_lock(&m_Mutex);
        m_Flag = true;

        pthread_cond_signal(&m_Condition);
        pthread_mutex_unlock(&m_Mutex);
    }

    inline void Wait()
    {
        if (m_Flag) return;

        pthread_mutex_lock(&m_Mutex);
        while(!m_Flag)
            pthread_cond_wait(&m_Condition, &m_Mutex);
        pthread_mutex_unlock(&m_Mutex);
    }

    consteval inline bool IsSet() const
    {
        return m_Flag;
    }

private:
    pthread_cond_t m_Condition;
    pthread_mutex_t m_Mutex;
    bool m_Flag;
};

}


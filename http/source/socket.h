#pragma once

#include <string_view>
#ifdef HTTP_WANT_TLS
#include <gnutls/gnutls.h>
#endif

#include <netinet/in.h>
#include <stdint.h>
#include <stddef.h>

namespace http
{

class ISocket
{
public:
    virtual ~ISocket() {}

    virtual void SetHandle(int fd) = 0;
    virtual void SetHostName(std::string_view host) noexcept {}
    virtual int Read(void* buffer, size_t bytes) = 0;
    virtual int Write(const void* buffer, size_t bytes) = 0;
};

class CHttpSocket : public ISocket
{
public:
    CHttpSocket() noexcept;
    ~CHttpSocket();
    void SetHandle(int fd) override;
    int Read(void* buffer, size_t bytes) override;
    int Write(const void* buffer, size_t bytes) override;

private:
    int m_Handle;
};

#ifdef HTTP_WANT_TLS

class CEncryptedSocket : public ISocket
{
public:
    CEncryptedSocket() noexcept;
    ~CEncryptedSocket();
    void SetHandle(int fd) override;
    void SetHostName(std::string_view host) noexcept override;
    int Read(void* buffer, size_t bytes) override;
    int Write(const void* buffer, size_t bytes) override;

private:
    int m_Handle;
    gnutls_session_t m_Session;
};

#endif

}

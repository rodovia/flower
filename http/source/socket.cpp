#include "socket.h"
#include "tls.h"
#include <gnutls/gnutls.h>
#include <assert.h>
#include <unistd.h>

#include <stdio.h>

http::CHttpSocket::CHttpSocket() noexcept
    : m_Handle(0)
{}

http::CHttpSocket::~CHttpSocket()
{
    if (m_Handle != 0)
        close(m_Handle);
}

void http::CHttpSocket::SetHandle(int fd)
{
    m_Handle = fd;
}

int http::CHttpSocket::Read(void* buffer, size_t size)
{
    return read(m_Handle, buffer, size);
}

int http::CHttpSocket::Write(const void* buffer, size_t size)
{
    return write(m_Handle, buffer, size);
}

#ifdef HTTP_WANT_TLS

http::CEncryptedSocket::CEncryptedSocket() noexcept
    : m_Handle(-1)
{
    gnutls_init(&m_Session, GNUTLS_CLIENT);
    gnutls_set_default_priority(m_Session);
    gnutls_credentials_set(m_Session, GNUTLS_CRD_CERTIFICATE, http::GetDefaultCert());
}

http::CEncryptedSocket::~CEncryptedSocket()
{
    gnutls_bye(m_Session, GNUTLS_SHUT_RDWR);
    gnutls_deinit(m_Session);
}

void http::CEncryptedSocket::SetHandle(int fd)
{
    int status = 0;
    gnutls_transport_set_int(m_Session, fd);
    gnutls_handshake_set_timeout(m_Session, 10000);

    do {
        status = gnutls_handshake(m_Session);
    } while(status < 0 && gnutls_error_is_fatal(status) == 0);
    m_Handle = fd;
}

int http::CEncryptedSocket::Read(void* buffer, size_t size)
{
    int value;
    do {
        value = gnutls_record_recv(m_Session, buffer, size);
    } while(value == GNUTLS_E_AGAIN || value == GNUTLS_E_INTERRUPTED);
    
    if (value < 0)
    {
        if (gnutls_error_is_fatal(value))
        {
            printf("*** GnuTLS Warning: ");
        }
        else 
        {
            printf("*** GnuTLS Error: ");
        }

        printf("%s\n", gnutls_strerror(value));
    }

    return value;
}

int http::CEncryptedSocket::Write(const void* buffer, size_t size)
{
    int value;
    do {
        value = gnutls_record_send(m_Session, buffer, size);
    } while(value == GNUTLS_E_AGAIN || value == GNUTLS_E_INTERRUPTED);
    return value;
}

void http::CEncryptedSocket::SetHostName(std::string_view host) noexcept
{
    gnutls_server_name_set(m_Session, GNUTLS_NAME_DNS, host.data(), host.length());
}

#endif

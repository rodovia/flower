#include "tls.h"
#include <stdio.h>

#ifdef HTTP_WANT_TLS
#include <gnutls/gnutls.h>

__attribute__((visibility("hidden"))) 
gnutls_certificate_credentials_t __ctx;
#endif

gnutls_certificate_credentials_t http::GetDefaultCert()
{
#ifdef HTTP_WANT_TLS
    return __ctx;
#else
    return nullptr;
#endif
}

void http::Initialize()
{
#ifdef HTTP_WANT_TLS
    int status;

    gnutls_certificate_allocate_credentials(&__ctx);
    gnutls_certificate_set_x509_system_trust(__ctx);
#endif
}

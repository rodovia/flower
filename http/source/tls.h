#pragma once

#ifdef HTTP_WANT_TLS
#include <gnutls/gnutls.h>
#else
using gnutls_certificate_credentials_t = void;
#endif

namespace http
{

gnutls_certificate_credentials_t GetDefaultCert();
void Initialize();

}

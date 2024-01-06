#pragma once

#ifdef __cplusplus
#define _EXTERN_C_OR_NOTHING extern "C" {
#define _RIGHT_BRACE_OR_NOTHING }
#else
#define _EXTERN_C_OR_NOTHING
#define _RIGHT_BRACE_OR_NOTHING
#endif

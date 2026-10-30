/*
 * Declarations for the real function capture helpers.
 */
#ifndef LIBTAS_DLREAL_H
#define LIBTAS_DLREAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Get the real getenv (from libSystem), bypassing any hook. */
char* getenv_from_real(const char *name);

/* Read environment variable directly from environ block, bypassing getenv entirely. */
const char* getenv_direct_call(const char *name);

#ifdef __cplusplus
}
#endif

#endif

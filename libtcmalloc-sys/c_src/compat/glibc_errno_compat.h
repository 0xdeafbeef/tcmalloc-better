#ifndef TCMALLOC_COMPAT_GLIBC_ERRNO_COMPAT_H_
#define TCMALLOC_COMPAT_GLIBC_ERRNO_COMPAT_H_

#ifndef __ASSEMBLER__
#include <errno.h>
#endif

#if defined(__linux__)
# undef EDOM
# define EDOM 33
# undef EILSEQ
# define EILSEQ 84
# undef ERANGE
# define ERANGE 34
#endif

#endif  /* TCMALLOC_COMPAT_GLIBC_ERRNO_COMPAT_H_ */

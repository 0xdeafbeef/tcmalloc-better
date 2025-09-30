#ifndef TCMALLOC_LIBC_OVERRIDE_H_
#define TCMALLOC_LIBC_OVERRIDE_H_

#include <new>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>

#ifndef EDOM
#define EDOM 33
#endif
#ifndef EILSEQ
#define EILSEQ 84
#endif
#ifndef ERANGE
#define ERANGE 34
#endif

#include "tcmalloc/tcmalloc.h"  // IWYU pragma: keep

#ifdef TCMALLOC_ALIAS
#undef TCMALLOC_ALIAS
#endif
#define TCMALLOC_ALIAS(tc_fn) \
  __attribute__((alias(#tc_fn), visibility("default")))

#ifdef TCMALLOC_NOTHROW
#undef TCMALLOC_NOTHROW
#endif
#define TCMALLOC_NOTHROW noexcept

extern "C" {

void* malloc(size_t size) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalMalloc);
void free(void* ptr) TCMALLOC_NOTHROW TCMALLOC_ALIAS(TCMallocInternalFree);
void free_sized(void* ptr, size_t size) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalFreeSized);
void free_aligned_sized(void* ptr, size_t align, size_t size) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalFreeAlignedSized);
void sdallocx(void* ptr, size_t size, int flags) noexcept
    TCMALLOC_ALIAS(TCMallocInternalSdallocx);
void* realloc(void* ptr, size_t size) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalRealloc);
void* reallocarray(void* ptr, size_t n, size_t size) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalReallocArray);
void* calloc(size_t n, size_t size) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalCalloc);
void cfree(void* ptr) TCMALLOC_NOTHROW TCMALLOC_ALIAS(TCMallocInternalCfree);
void* memalign(size_t align, size_t s) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalMemalign);
void* aligned_alloc(size_t align, size_t s) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalAlignedAlloc);
void* valloc(size_t size) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalValloc);
void* pvalloc(size_t size) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalPvalloc);
int posix_memalign(void** r, size_t a, size_t s) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalPosixMemalign);
void malloc_stats(void) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalMallocStats);
int malloc_trim(size_t pad) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalMallocTrim);
int mallopt(int cmd, int value) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalMallOpt);
#ifdef TCMALLOC_HAVE_STRUCT_MALLINFO
struct mallinfo mallinfo(void) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalMallInfo);
#endif
#ifdef TCMALLOC_HAVE_STRUCT_MALLINFO2
struct mallinfo2 mallinfo2(void) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalMallInfo2);
#endif
int malloc_info(int opts, FILE* fp) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalMallocInfo);
size_t malloc_size(void* p) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalMallocSize);
size_t malloc_usable_size(void* p) TCMALLOC_NOTHROW
    TCMALLOC_ALIAS(TCMallocInternalMallocSize);

}  // extern "C"

#if defined(__cplusplus) && defined(BRIDGE_OVERRIDE_CPP_OPERATORS)

//void* operator new(size_t size) noexcept(false)
//    TCMALLOC_ALIAS(TCMallocInternalNew);
//void operator delete(void* p) noexcept
//    TCMALLOC_ALIAS(TCMallocInternalDelete);
////void operator delete(void* p, size_t size) noexcept temp hack
////    TCMALLOC_ALIAS(TCMallocInternalDeleteSized);
//void operator delete(void* p, size_t size) noexcept
//    TCMALLOC_ALIAS(TCMallocInternalDelete);
//void* operator new[](size_t size) noexcept(false)
//    TCMALLOC_ALIAS(TCMallocInternalNewArray);
//void operator delete[](void* p) noexcept
//    TCMALLOC_ALIAS(TCMallocInternalDeleteArray);
////void operator delete[](void* p, size_t size) noexcept
////    TCMALLOC_ALIAS(TCMallocInternalDeleteArraySized);
//void operator delete[](void* p, size_t size) noexcept
//    TCMALLOC_ALIAS(TCMallocInternalDeleteArray);
//void* operator new(size_t size, const std::nothrow_t& nt) noexcept
//    TCMALLOC_ALIAS(TCMallocInternalNewNothrow);
//void* operator new[](size_t size, const std::nothrow_t& nt) noexcept
//    TCMALLOC_ALIAS(TCMallocInternalNewArrayNothrow);
//void operator delete(void* p, const std::nothrow_t& nt) noexcept
//    TCMALLOC_ALIAS(TCMallocInternalDeleteNothrow);
//void operator delete[](void* p, const std::nothrow_t& nt) noexcept
//    TCMALLOC_ALIAS(TCMallocInternalDeleteArrayNothrow);
//
//void* operator new(size_t size, std::align_val_t alignment) noexcept(false)
//    TCMALLOC_ALIAS(TCMallocInternalNewAligned);
//void* operator new(size_t size, std::align_val_t alignment,
//                   const std::nothrow_t&) noexcept
//    TCMALLOC_ALIAS(TCMallocInternalNewAlignedNothrow);
//void operator delete(void* p, std::align_val_t alignment) noexcept
//    TCMALLOC_ALIAS(TCMallocInternalDeleteAligned);
//void operator delete(void* p, std::align_val_t alignment,
//                     const std::nothrow_t&) noexcept
//    TCMALLOC_ALIAS(TCMallocInternalDeleteAlignedNothrow);
////void operator delete(void* p, size_t size, std::align_val_t alignment) noexcept
////    TCMALLOC_ALIAS(TCMallocInternalDeleteSizedAligned);
//void operator delete(void* p, size_t size, std::align_val_t alignment) noexcept
//    TCMALLOC_ALIAS(TCMallocInternalDeleteSizedAligned);
//void* operator new[](size_t size, std::align_val_t alignment) noexcept(false)
//    TCMALLOC_ALIAS(TCMallocInternalNewArrayAligned);
//void* operator new[](size_t size, std::align_val_t alignment,
//                     const std::nothrow_t&) noexcept
//    TCMALLOC_ALIAS(TCMallocInternalNewArrayAlignedNothrow);
//void operator delete[](void* p, std::align_val_t alignment) noexcept
//    TCMALLOC_ALIAS(TCMallocInternalDeleteArrayAligned);
//void operator delete[](void* p, std::align_val_t alignment,
//                       const std::nothrow_t&) noexcept
//    TCMALLOC_ALIAS(TCMallocInternalDeleteArrayAlignedNothrow);
////void operator delete[](void* p, size_t size,
////                       std::align_val_t alignment) noexcept
////    TCMALLOC_ALIAS(TCMallocInternalDeleteArraySizedAligned);
//void operator delete[](void* p, size_t size,
//                       std::align_val_t alignment) noexcept
//    TCMALLOC_ALIAS(TCMallocInternalDeleteArrayAligned);
//

//#include <new>
void operator delete(void* p, std::size_t) noexcept { ::operator delete(p); }
void operator delete[](void* p, std::size_t) noexcept { ::operator delete[](p); }
void operator delete(void* p, std::align_val_t) noexcept { ::operator delete(p); }
void operator delete[](void* p, std::align_val_t) noexcept { ::operator delete[](p); }
void operator delete(void* p, std::size_t, std::align_val_t) noexcept { ::operator delete(p); }
void operator delete[](void* p, std::size_t, std::align_val_t) noexcept { ::operator delete[](p); }


#endif  // defined(__cplusplus) && defined(BRIDGE_OVERRIDE_CPP_OPERATORS)
#endif  // TCMALLOC_LIBC_OVERRIDE_H_

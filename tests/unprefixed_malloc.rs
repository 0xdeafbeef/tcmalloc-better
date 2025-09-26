#![cfg(feature = "unprefixed_malloc_on_supported_platforms")]
//! Port of tikv-jemalloc-sys/tests/unprefixed_malloc.rs.

use tcmalloc_better::TCMalloc;

type MallocFn = unsafe extern "C" fn(libc::size_t) -> *mut core::ffi::c_void;

#[global_allocator]
static GLOBAL: TCMalloc = TCMalloc;

#[test]
fn malloc_is_overridden() {
    let tc_malloc: MallocFn = libtcmalloc_sys::malloc;
    let libc_malloc: MallocFn = libc::malloc;
    assert_eq!(tc_malloc as usize, libc_malloc as usize);
}

#[test]
fn malloc_and_libc_are_interoperable_when_overridden() {
    let ptr = unsafe { libtcmalloc_sys::malloc(42) };
    assert!(!ptr.is_null());
    unsafe { libc::free(ptr) };
}

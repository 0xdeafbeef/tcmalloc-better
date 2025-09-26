use crate::TCMalloc;
use libtcmalloc_sys::{NeedsProcessBackgroundActions, ProcessBackgroundActions};
#[cfg(feature = "std")]
use std::thread;

#[cfg(feature = "std")]
extern crate std;

#[cfg(all(feature = "extension", feature = "std"))]
use core::ffi::c_char;
#[cfg(all(feature = "extension", feature = "std"))]
use core::fmt;
#[cfg(all(feature = "extension", feature = "std"))]
use std::ffi::CString;
#[cfg(all(feature = "extension", feature = "std"))]
use std::vec::Vec;

#[cfg(feature = "extension")]
#[cfg_attr(docsrs, doc(cfg(feature = "extension")))]
impl TCMalloc {
    /// Return true if `process_background_actions` should be called on this platform.
    #[inline]
    pub fn needs_process_background_actions() -> bool {
        unsafe { NeedsProcessBackgroundActions() }
    }

    /// Runs housekeeping actions for the allocator off of the main allocation path.
    ///
    /// Should be run in the background thread. May return or may not return.
    /// Use `process_background_actions_thread()` if possible.
    #[inline]
    pub fn process_background_actions() {
        unsafe { ProcessBackgroundActions() };
    }

    /// Runs housekeeping actions for the allocator in the background thread.
    #[cfg(feature = "std")]
    #[cfg_attr(docsrs, doc(cfg(feature = "std")))]
    pub fn process_background_actions_thread() -> Option<thread::JoinHandle<()>> {
        if Self::needs_process_background_actions() {
            Some(thread::spawn(|| {
                Self::process_background_actions();
            }))
        } else {
            None
        }
    }
}

#[cfg(all(feature = "extension", feature = "std"))]
#[derive(Clone, Copy, Debug, Default)]
pub struct AllocatorMetrics {
    pub allocated: usize,
    pub active: usize,
    pub metadata: usize,
    pub resident: usize,
    pub mapped: usize,
    pub retained: usize,
    pub dirty: usize,
    pub fragmentation: usize,
}

#[cfg(all(feature = "extension", feature = "std"))]
#[derive(Debug)]
pub struct AllocatorMetricsError(&'static str);

#[cfg(all(feature = "extension", feature = "std"))]
impl fmt::Display for AllocatorMetricsError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "failed to read {}", self.0)
    }
}

#[cfg(all(feature = "extension", feature = "std"))]
impl std::error::Error for AllocatorMetricsError {}

#[cfg(all(feature = "extension", feature = "std"))]
#[derive(Debug)]
pub struct MetricsRawError(&'static str);

#[cfg(all(feature = "extension", feature = "std"))]
impl fmt::Display for MetricsRawError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "failed to fetch raw allocator metrics: {}", self.0)
    }
}

#[cfg(all(feature = "extension", feature = "std"))]
impl std::error::Error for MetricsRawError {}

#[cfg(all(feature = "extension", feature = "std"))]
pub fn fetch_allocator_metrics() -> Result<AllocatorMetrics, AllocatorMetricsError> {
    Ok(AllocatorMetrics {
        allocated: read_numeric("generic.current_allocated_bytes")?,
        active: read_numeric("generic.heap_size")?,
        metadata: read_numeric("tcmalloc.metadata_bytes")?,
        resident: read_numeric("generic.physical_memory_used")?,
        mapped: read_numeric("generic.virtual_memory_used")?,
        retained: read_numeric("tcmalloc.pageheap_unmapped_bytes")?,
        dirty: read_numeric("tcmalloc.pageheap_free_bytes")?,
        fragmentation: read_numeric("tcmalloc.external_fragmentation_bytes")?,
    })
}

#[cfg(all(feature = "extension", feature = "std"))]
pub fn metrics_raw() -> Result<Vec<u8>, MetricsRawError> {
    let mut data_ptr: *const c_char = core::ptr::null();
    let mut data_len = 0usize;
    let ok = unsafe { libtcmalloc_sys::BridgeGetStats(&mut data_ptr, &mut data_len) };
    if !ok {
        return Err(MetricsRawError("get stats"));
    }

    unsafe {
        let outcome = if data_len == 0 {
            Ok(Vec::new())
        } else if data_ptr.is_null() {
            Err(MetricsRawError("null pointer"))
        } else {
            let slice = core::slice::from_raw_parts(data_ptr as *const u8, data_len);
            Ok(slice.to_vec())
        };
        libtcmalloc_sys::BridgeFreeStats(data_ptr, data_len);
        outcome
    }
}

#[cfg(all(feature = "extension", feature = "std"))]
fn read_numeric(property: &'static str) -> Result<usize, AllocatorMetricsError> {
    let name = CString::new(property).expect("property name contains null byte");
    let mut value = 0usize;
    let ok = unsafe {
        libtcmalloc_sys::BridgeGetNumericProperty(name.as_ptr(), property.len(), &mut value)
    };
    if ok {
        Ok(value)
    } else {
        Err(AllocatorMetricsError(property))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    #[cfg(target_os = "linux")]
    fn test_needs_process_background_actions() {
        assert!(!TCMalloc::needs_process_background_actions());
    }

    #[test]
    #[cfg(feature = "std")]
    fn test_process_background_actions() {
        TCMalloc::process_background_actions_thread();
    }

    #[test]
    #[cfg(feature = "std")]
    fn it_fetches_allocator_metrics() {
        use core::alloc::GlobalAlloc;
        let before = fetch_allocator_metrics().expect("failed to fetch initial metrics");
        let layout = core::alloc::Layout::from_size_align(1 << 20, 16).unwrap();
        let allocator = TCMalloc;
        let ptr = unsafe { allocator.alloc(layout) };
        assert!(!ptr.is_null());

        let mut attempt = 0;
        let mut after = None;
        while attempt < 16 {
            let snapshot =
                fetch_allocator_metrics().expect("failed to fetch post-allocation metrics");
            if snapshot.allocated >= before.allocated + layout.size() {
                after = Some(snapshot);
                break;
            }
            attempt += 1;
            std::thread::sleep(std::time::Duration::from_millis(10));
        }

        unsafe { allocator.dealloc(ptr, layout) };

        let after = after.expect("allocator metrics did not reflect allocation");
        assert!(after.allocated >= before.allocated + layout.size());
    }

    #[test]
    #[cfg(feature = "std")]
    fn it_fetches_raw_metrics() {
        metrics_raw().expect("failed to fetch raw metrics");
    }
}

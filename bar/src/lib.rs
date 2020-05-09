use std::ffi::*;
use std::os::raw::c_char;

#[no_mangle]
pub extern "C" fn get_text() -> *const c_char {
    CString::new("WORKS!").expect("CString::new failed.").as_ptr()
}

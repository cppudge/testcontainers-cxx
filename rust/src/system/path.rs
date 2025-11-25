use std::path::PathBuf;

pub struct RsPath {
    pub path: PathBuf,
}

pub fn rs_path_from_bytes(b: &[u8]) -> Box<RsPath> {
    #[cfg(target_os = "windows")]
    {
        use std::os::windows::ffi::OsStringExt;
        let utf16_vec: Vec<u16> = String::from_utf8_lossy(b).encode_utf16().collect();
        let os_str = std::ffi::OsString::from_wide(&utf16_vec);
        Box::new(RsPath::new(PathBuf::from(os_str)))
    }
    #[cfg(not(target_os = "windows"))]
    {
        use std::os::unix::ffi::OsStrExt;
        let os_str = std::ffi::OsStr::from_bytes(b);
        Box::new(RsPath::new(PathBuf::from(os_str)))
    }
}

pub fn rs_path_from_utf16(w: &[u16]) -> Box<RsPath> {
    #[cfg(target_os = "windows")]
    {
        use std::os::windows::ffi::OsStringExt;
        let os_str = std::ffi::OsString::from_wide(w);
        Box::new(RsPath::new(PathBuf::from(os_str)))
    }
    #[cfg(not(target_os = "windows"))]
    {
        let utf8_string = String::from_utf16_lossy(w);
        Box::new(RsPath::new(PathBuf::from(utf8_string)))
    }
}

pub fn rs_path_destroy(path: Box<RsPath>) {
    drop(path);
}

impl RsPath {
    pub fn new(path: PathBuf) -> Self {
        Self { path }
    }

    pub fn rs_path_to_string_opt(self: &RsPath) -> Result<Vec<String>, String> {
        Ok(self.path
            .to_str()
            .map(|s| s.to_string())
            .into_iter()
            .collect())
    }
}

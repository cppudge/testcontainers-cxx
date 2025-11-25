use testcontainers::core::{AccessMode, Mount};

pub struct RsMount {
    mount: Mount,
}

pub fn rs_mount_bind_mount(host_path: String, container_path: String) -> Box<RsMount> {
    Box::new(RsMount::new(Mount::bind_mount(host_path, container_path)))
}

pub fn rs_mount_volume_mount(name: String, container_path: String) -> Box<RsMount> {
    Box::new(RsMount::new(Mount::volume_mount(name, container_path)))
}

pub fn rs_mount_tmpfs_mount(container_path: String) -> Box<RsMount> {
    Box::new(RsMount::new(Mount::tmpfs_mount(container_path)))
}

pub fn rs_mount_with_read_only(mount: Box<RsMount>) -> Box<RsMount> {
    Box::new(RsMount::new(mount.mount.with_access_mode(AccessMode::ReadOnly)))
}

pub fn rs_mount_with_read_write(mount: Box<RsMount>) -> Box<RsMount> {
    Box::new(RsMount::new(mount.mount.with_access_mode(AccessMode::ReadWrite)))
}

pub fn rs_mount_with_size_bytes(mount: Box<RsMount>, size: i64) -> Box<RsMount> {
    Box::new(RsMount::new(mount.mount.with_size_bytes(size)))
}

pub fn rs_mount_with_mode(mount: Box<RsMount>, mode: i64) -> Box<RsMount> {
    Box::new(RsMount::new(mount.mount.with_mode(mode)))
}

pub fn rs_mount_destroy(mount: Box<RsMount>) {
    drop(mount);
}

impl RsMount {
    pub fn new(mount: Mount) -> Self {
        Self { mount }
    }
}

impl Into<Mount> for RsMount {
    fn into(self) -> Mount {
        self.mount
    }
}

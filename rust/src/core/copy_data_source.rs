use testcontainers::core::CopyDataSource;
use crate::system::path::RsPath;

pub struct RsCopyDataSource {
    source: CopyDataSource,
}

pub fn rs_copy_data_source_file(path: Box<RsPath>) -> Box<RsCopyDataSource> {
    Box::new(RsCopyDataSource::new(CopyDataSource::File(path.path)))
}

pub fn rs_copy_data_source_data(data: Vec<u8>) -> Box<RsCopyDataSource> {
    Box::new(RsCopyDataSource::new(CopyDataSource::Data(data)))
}

pub fn rs_copy_data_source_destroy(source: Box<RsCopyDataSource>) {
    drop(source);
}

impl RsCopyDataSource {
    pub fn new(source: CopyDataSource) -> Self {
        Self { source }
    }
}

impl Into<CopyDataSource> for RsCopyDataSource {
    fn into(self) -> CopyDataSource {
        self.source
    }
}


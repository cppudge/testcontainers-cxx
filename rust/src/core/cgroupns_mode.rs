use testcontainers::core::CgroupnsMode;

pub struct RsCgroupnsMode {
	mode: CgroupnsMode,
}

pub fn rs_cgroupns_mode_private() -> Box<RsCgroupnsMode> {
	Box::new(RsCgroupnsMode::new(CgroupnsMode::Private))
}

pub fn rs_cgroupns_mode_host() -> Box<RsCgroupnsMode> {
	Box::new(RsCgroupnsMode::new(CgroupnsMode::Host))
}

pub fn rs_cgroupns_mode_destroy(mode: Box<RsCgroupnsMode>) {
	drop(mode);
}

impl RsCgroupnsMode {
	pub fn new(mode: CgroupnsMode) -> Self {
		Self { mode }
	}
}

impl Into<CgroupnsMode> for RsCgroupnsMode {
	fn into(self) -> CgroupnsMode {
		self.mode
	}
}


use testcontainers::core::Host;

pub struct RsHost {
    host: Host,
}

pub fn rs_host_addr(addr: String) -> Result<Box<RsHost>, String> {
    addr.parse()
        .map_err(|e| format!("Failed to parse host address: {}", e))
        .map(|host| Box::new(RsHost::new(Host::Addr(host))))
}

pub fn rs_host_host_gateway_linux() -> Box<RsHost> {
    Box::new(RsHost::new(Host::HostGateway))
}

pub fn rs_host_destroy(host: Box<RsHost>) {
    drop(host);
}

impl RsHost {
    pub fn new(host: Host) -> Self {
        Self { host }
    }
}

impl Into<Host> for RsHost {
    fn into(self) -> Host {
        self.host
    }
}

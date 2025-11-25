use crate::{system::ip::ipv4_addr::RsIpv4Addr, system::ip::ipv6_addr::RsIpv6Addr};
use url::Host;

pub struct RsUrlHost {
    pub host: Host<String>,
}

pub fn rs_url_host_domain(domain: String) -> Box<RsUrlHost> {
    Box::new(RsUrlHost::new(Host::Domain(domain)))
}

pub fn rs_url_host_from_ipv4(addr: Box<RsIpv4Addr>) -> Box<RsUrlHost> {
    Box::new(RsUrlHost::new(Host::Ipv4(addr.addr)))
}

pub fn rs_url_host_from_ipv6(addr: Box<RsIpv6Addr>) -> Box<RsUrlHost> {
    Box::new(RsUrlHost::new(Host::Ipv6(addr.addr)))
}

pub fn rs_url_host_destroy(host: Box<RsUrlHost>) {
    drop(host);
}

impl RsUrlHost {
    pub fn new(host: Host<String>) -> Self {
        Self { host }
    }

    pub fn rs_url_host_is_domain(&self) -> bool {
        matches!(self.host, Host::Domain(_))
    }

    pub fn rs_url_host_is_ipv4(&self) -> bool {
        matches!(self.host, Host::Ipv4(_))
    }

    pub fn rs_url_host_is_ipv6(&self) -> bool {
        matches!(self.host, Host::Ipv6(_))
    }

    pub fn rs_url_host_to_domain(&self) -> Result<String, String> {
        match &self.host {
            Host::Domain(s) => Ok(s.clone()),
            _ => Err("Not a domain".to_string()),
        }
    }

    pub fn rs_url_host_to_ipv4(&self) -> Result<Box<RsIpv4Addr>, String> {
        match &self.host {
            Host::Ipv4(addr) => Ok(Box::new(RsIpv4Addr::new(*addr))),
            _ => Err("Not an IPv4 address".to_string()),
        }
    }

    pub fn rs_url_host_to_ipv6(&self) -> Result<Box<RsIpv6Addr>, String> {
        match &self.host {
            Host::Ipv6(addr) => Ok(Box::new(RsIpv6Addr::new(*addr))),
            _ => Err("Not an IPv6 address".to_string()),
        }
    }

    pub fn rs_url_host_to_string(&self) -> String {
        self.host.to_string()
    }
}


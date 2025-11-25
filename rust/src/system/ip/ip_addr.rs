use crate::{system::ip::ipv4_addr::RsIpv4Addr, system::ip::ipv6_addr::RsIpv6Addr};
use std::net::IpAddr;

pub struct RsIpAddr {
    pub addr: IpAddr,
}

pub fn rs_ip_addr_from_ipv4(ipv4: Box<RsIpv4Addr>) -> Box<RsIpAddr> {
    Box::new(RsIpAddr::new(IpAddr::V4(ipv4.addr)))
}

pub fn rs_ip_addr_from_ipv6(ipv6: Box<RsIpv6Addr>) -> Box<RsIpAddr> {
    Box::new(RsIpAddr::new(IpAddr::V6(ipv6.addr)))
}

pub fn rs_ip_addr_destroy(addr: Box<RsIpAddr>) {
    drop(addr);
}

impl RsIpAddr {
    pub fn new(addr: IpAddr) -> Self {
        Self { addr }
    }

    pub fn rs_ip_addr_is_unspecified(&self) -> bool {
        self.addr.is_unspecified()
    }

    pub fn rs_ip_addr_is_loopback(&self) -> bool {
        self.addr.is_loopback()
    }

    // pub fn rs_ip_addr_is_global(&self) -> bool {
    //     self.addr.is_global()
    // }

    pub fn rs_ip_addr_is_multicast(&self) -> bool {
        self.addr.is_multicast()
    }

    // pub fn rs_ip_addr_is_documentation(&self) -> bool {
    //     self.addr.is_documentation()
    // }

    // pub fn rs_ip_addr_is_benchmarking(&self) -> bool {
    //     self.addr.is_benchmarking()
    // }

    pub fn rs_ip_addr_is_ipv4(&self) -> bool {
        self.addr.is_ipv4()
    }

    pub fn rs_ip_addr_is_ipv6(&self) -> bool {
        self.addr.is_ipv6()
    }

    pub fn rs_ip_addr_to_canonical(&self) -> Box<RsIpAddr> {
        Box::new(RsIpAddr::new(self.addr.to_canonical()))
    }

    pub fn rs_ip_addr_to_ipv4(&self) -> Result<Box<RsIpv4Addr>, String> {
        match self.addr {
            IpAddr::V4(v4) => Ok(Box::new(RsIpv4Addr::new(v4))),
            IpAddr::V6(_) => Err("Address is not IPv4".to_string()),
        }
    }

    pub fn rs_ip_addr_to_ipv6(&self) -> Result<Box<RsIpv6Addr>, String> {
        match self.addr {
            IpAddr::V4(_) => Err("Address is not IPv6".to_string()),
            IpAddr::V6(v6) => Ok(Box::new(RsIpv6Addr::new(v6))),
        }
    }

    pub fn rs_ip_addr_to_string(&self) -> String {
        self.addr.to_string()
    }
}


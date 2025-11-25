use crate::system::ip::ip_addr::RsIpAddr;
use std::net::Ipv6Addr;

pub struct RsIpv6Addr {
    pub addr: Ipv6Addr,
}

pub fn rs_ipv6_addr_new(a: u16, b: u16, c: u16, d: u16, e: u16, f: u16, g: u16, h: u16) -> Box<RsIpv6Addr> {
    Box::new(RsIpv6Addr::new(Ipv6Addr::new(a, b, c, d, e, f, g, h)))
}

pub fn rs_ipv6_addr_localhost() -> Box<RsIpv6Addr> {
    Box::new(RsIpv6Addr::new(Ipv6Addr::LOCALHOST))
}

pub fn rs_ipv6_addr_unspecified() -> Box<RsIpv6Addr> {
    Box::new(RsIpv6Addr::new(Ipv6Addr::UNSPECIFIED))
}

pub fn rs_ipv6_addr_from_bits(bits_low: u64, bits_high: u64) -> Box<RsIpv6Addr> {
    Box::new(RsIpv6Addr::new(Ipv6Addr::from_bits(u128::from(bits_low) << 64 | u128::from(bits_high))))
}

pub fn rs_ipv6_addr_from_segments(segments: [u16; 8]) -> Box<RsIpv6Addr> {
    Box::new(RsIpv6Addr::new(Ipv6Addr::from(segments)))
}

pub fn rs_ipv6_addr_from_octets(octets: [u8; 16]) -> Box<RsIpv6Addr> {
    Box::new(RsIpv6Addr::new(Ipv6Addr::from(octets)))
}

pub fn rs_ipv6_addr_destroy(addr: Box<RsIpv6Addr>) {
    drop(addr);
}

impl RsIpv6Addr {
    pub fn new(addr: Ipv6Addr) -> Self {
        Self { addr }
    }

    pub fn rs_ipv6_addr_to_bits_pair(&self) -> [u64; 2] {
        let bits = self.addr.to_bits();
        [(bits >> 64) as u64, bits as u64]
    }

    pub fn rs_ipv6_addr_segments(&self) -> [u16; 8] {
        self.addr.segments()
    }

    pub fn rs_ipv6_addr_octets(&self) -> [u8; 16] {
        self.addr.octets()
    }

    pub fn rs_ipv6_addr_is_unspecified(&self) -> bool {
        self.addr.is_unspecified()
    }

    pub fn rs_ipv6_addr_is_loopback(&self) -> bool {
        self.addr.is_loopback()
    }

    // pub fn rs_ipv6_addr_is_global(&self) -> bool {
    //     self.addr.is_global()
    // }

    // pub fn rs_ipv6_addr_is_unicast(&self) -> bool {
    //     self.addr.is_unicast()
    // }

    pub fn rs_ipv6_addr_is_unicast_link_local(&self) -> bool {
        self.addr.is_unicast_link_local()
    }

    pub fn rs_ipv6_addr_is_unique_local(&self) -> bool {
        self.addr.is_unique_local()
    }

    // pub fn rs_ipv6_addr_is_documentation(&self) -> bool {
    //     self.addr.is_documentation()
    // }

    // pub fn rs_ipv6_addr_is_benchmarking(&self) -> bool {
    //     self.addr.is_benchmarking()
    // }

    // pub fn rs_ipv6_addr_is_unicast_global(&self) -> bool {
    //     self.addr.is_unicast_global()
    // }

    // pub fn rs_ipv6_addr_multicast_scope(&self) -> u8 {
    //     match self.addr.multicast_scope() {
    //         Some(scope) => match scope {
    //             std::net::Ipv6MulticastScope::InterfaceLocal => 1,
    //             std::net::Ipv6MulticastScope::LinkLocal => 2,
    //             std::net::Ipv6MulticastScope::RealmLocal => 3,
    //             std::net::Ipv6MulticastScope::AdminLocal => 4,
    //             std::net::Ipv6MulticastScope::SiteLocal => 5,
    //             std::net::Ipv6MulticastScope::OrganizationLocal => 8,
    //             std::net::Ipv6MulticastScope::Global => 14,
    //         },
    //         None => 0,
    //     }
    // }

    pub fn rs_ipv6_addr_is_multicast(&self) -> bool {
        self.addr.is_multicast()
    }

    // pub fn rs_ipv6_addr_is_ipv4_mapped(&self) -> bool {
    //     self.addr.is_ipv4_mapped()
    // }

    pub fn rs_ipv6_addr_to_canonical(&self) -> Box<RsIpAddr> {
        Box::new(RsIpAddr::new(self.addr.to_canonical()))
    }

    pub fn rs_ipv6_addr_to_string(&self) -> String {
        self.addr.to_string()
    }
}


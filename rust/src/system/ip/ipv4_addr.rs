use std::net::Ipv4Addr;

pub struct RsIpv4Addr {
    pub addr: Ipv4Addr,
}

pub fn rs_ipv4_addr_new(a: u8, b: u8, c: u8, d: u8) -> Box<RsIpv4Addr> {
    Box::new(RsIpv4Addr::new(Ipv4Addr::new(a, b, c, d)))
}

pub fn rs_ipv4_addr_localhost() -> Box<RsIpv4Addr> {
    Box::new(RsIpv4Addr::new(Ipv4Addr::LOCALHOST))
}

pub fn rs_ipv4_addr_unspecified() -> Box<RsIpv4Addr> {
    Box::new(RsIpv4Addr::new(Ipv4Addr::UNSPECIFIED))
}

pub fn rs_ipv4_addr_broadcast() -> Box<RsIpv4Addr> {
    Box::new(RsIpv4Addr::new(Ipv4Addr::BROADCAST))
}

pub fn rs_ipv4_addr_from_bits(bits: u32) -> Box<RsIpv4Addr> {
    Box::new(RsIpv4Addr::new(Ipv4Addr::from_bits(bits)))
}

pub fn rs_ipv4_addr_from_octets(octets: [u8; 4]) -> Box<RsIpv4Addr> {
    Box::new(RsIpv4Addr::new(Ipv4Addr::from(octets)))
}

pub fn rs_ipv4_addr_destroy(addr: Box<RsIpv4Addr>) {
    drop(addr);
}

impl RsIpv4Addr {
    pub fn new(addr: Ipv4Addr) -> Self {
        Self { addr }
    }

    pub fn rs_ipv4_addr_to_bits(&self) -> u32 {
        self.addr.to_bits()
    }

    pub fn rs_ipv4_addr_octets(&self) -> [u8; 4] {
        self.addr.octets()
    }

    pub fn rs_ipv4_addr_is_unspecified(&self) -> bool {
        self.addr.is_unspecified()
    }

    pub fn rs_ipv4_addr_is_loopback(&self) -> bool {
        self.addr.is_loopback()
    }

    pub fn rs_ipv4_addr_is_private(&self) -> bool {
        self.addr.is_private()
    }

    pub fn rs_ipv4_addr_is_link_local(&self) -> bool {
        self.addr.is_link_local()
    }

    // pub fn rs_ipv4_addr_is_global(&self) -> bool {
    //     self.addr.is_global()
    // }

    // pub fn rs_ipv4_addr_is_shared(&self) -> bool {
    //     self.addr.is_shared()
    // }

    // pub fn rs_ipv4_addr_is_benchmarking(&self) -> bool {
    //     self.addr.is_benchmarking()
    // }

    // pub fn rs_ipv4_addr_is_reserved(&self) -> bool {
    //     self.addr.is_reserved()
    // }

    pub fn rs_ipv4_addr_is_multicast(&self) -> bool {
        self.addr.is_multicast()
    }

    pub fn rs_ipv4_addr_is_broadcast(&self) -> bool {
        self.addr.is_broadcast()
    }

    pub fn rs_ipv4_addr_is_documentation(&self) -> bool {
        self.addr.is_documentation()
    }

    pub fn rs_ipv4_addr_to_string(&self) -> String {
        self.addr.to_string()
    }
}


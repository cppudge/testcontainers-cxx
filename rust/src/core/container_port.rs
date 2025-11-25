use testcontainers::core::ContainerPort;

pub struct RsContainerPort {
	port: ContainerPort,
}

pub fn rs_container_port_tcp(port: u16) -> Box<RsContainerPort> {
	Box::new(RsContainerPort::new(ContainerPort::Tcp(port)))
}

pub fn rs_container_port_udp(port: u16) -> Box<RsContainerPort> {
	Box::new(RsContainerPort::new(ContainerPort::Udp(port)))
}

pub fn rs_container_port_sctp(port: u16) -> Box<RsContainerPort> {
	Box::new(RsContainerPort::new(ContainerPort::Sctp(port)))
}

pub fn rs_container_port_destroy(port: Box<RsContainerPort>) {
	drop(port);
}

pub fn rs_container_port_vec_pop(vec: &mut Vec<RsContainerPort>) -> Result<Box<RsContainerPort>, String> {
	vec.pop()
		.map(Box::new)
		.ok_or_else(|| "Vector is empty".to_string())
}

impl RsContainerPort {
	pub fn new(port: ContainerPort) -> Self {
		Self { port }
	}

	pub fn rs_container_port_as_u16(&self) -> u16 {
		match self.port {
			ContainerPort::Tcp(port) => port,
			ContainerPort::Udp(port) => port,
			ContainerPort::Sctp(port) => port,
		}
	}

	pub fn rs_container_port_is_tcp(&self) -> bool {
		matches!(self.port, ContainerPort::Tcp(_))
	}

	pub fn rs_container_port_is_udp(&self) -> bool {
		matches!(self.port, ContainerPort::Udp(_))
	}

	pub fn rs_container_port_is_sctp(&self) -> bool {
		matches!(self.port, ContainerPort::Sctp(_))
	}
}

impl Into<ContainerPort> for RsContainerPort {
	fn into(self) -> ContainerPort {
		self.port
	}
}

impl From<ContainerPort> for RsContainerPort {
	fn from(port: ContainerPort) -> Self {
		Self::new(port)
	}
}

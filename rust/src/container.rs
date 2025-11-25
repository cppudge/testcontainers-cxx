use crate::{
    core::exec::exec_command::RsExecCommand, core::exec::sync_exec_result::RsSyncExecResult,
    image::RsGenericImage, system::ip::ip_addr::RsIpAddr, system::url_host::RsUrlHost,
    core::container_port::RsContainerPort,
};
use testcontainers::{core::ContainerPort, Container, GenericImage};

pub struct RsContainer {
    container: Container<GenericImage>,
}

pub fn rs_container_destroy(container: Box<RsContainer>) {
    drop(container);
}

pub fn rs_container_rm(container: Box<RsContainer>) -> Result<(), String> {
    container
        .container
        .rm()
        .map_err(|e| format!("Failed to remove container: {}", e))
}

impl RsContainer {
    pub fn new(container: Container<GenericImage>) -> Self {
        Self { container }
    }

    pub fn rs_container_id(self: &RsContainer) -> &str {
        self.container.id()
    }

    pub fn rs_container_image(self: &RsContainer) -> Box<RsGenericImage> {
        Box::new(RsGenericImage::new(self.container.image().clone()))
    }

    // TODO HashMap<T,U>
    // pub fn ports(&self) -> Result<Ports>

    pub fn rs_container_get_host_port_ipv4(
        self: &RsContainer,
        port: Box<RsContainerPort>,
    ) -> Result<u16, String> {
        let port: ContainerPort = (*port).into();
        self.container
            .get_host_port_ipv4(port)
            .map_err(|e| format!("Failed to get host port: {}", e))
    }

    pub fn rs_container_get_host_port_ipv6(
        self: &RsContainer,
        port: Box<RsContainerPort>,
    ) -> Result<u16, String> {
        let port: ContainerPort = (*port).into();
        self.container
            .get_host_port_ipv6(port)
            .map_err(|e| format!("Failed to get host port: {}", e))
    }

    pub fn rs_container_get_bridge_ip_address(self: &RsContainer) -> Result<Box<RsIpAddr>, String> {
        self.container
            .get_bridge_ip_address()
            .map_err(|e| format!("Failed to get bridge IP address: {}", e))
            .map(|ip_addr| Box::new(RsIpAddr::new(ip_addr)))
    }

    pub fn rs_container_get_host(self: &RsContainer) -> Result<Box<RsUrlHost>, String> {
        self.container
            .get_host()
            .map_err(|e| format!("Failed to get host: {}", e))
            .map(|host| Box::new(RsUrlHost::new(host)))
    }

    pub fn rs_container_exec(
        self: &RsContainer,
        cmd: Box<RsExecCommand>,
    ) -> Result<Box<RsSyncExecResult>, String> {
        self.container
            .exec(cmd.command)
            .map_err(|e| format!("Failed to exec command: {}", e))
            .map(|exec_result| Box::new(RsSyncExecResult::new(exec_result)))
    }

    pub fn rs_container_stop(self: &RsContainer) -> Result<(), String> {
        self.container
            .stop()
            .map_err(|e| format!("Failed to stop container: {}", e))
    }

    pub fn rs_container_stop_with_timeout(
        self: &RsContainer,
        timeout_sec_opt: Vec<i32>,
    ) -> Result<(), String> {
        self.container
            .stop_with_timeout(timeout_sec_opt.first().copied())
            .map_err(|e| format!("Failed to stop container with timeout: {}", e))
    }

    pub fn rs_container_start(self: &RsContainer) -> Result<(), String> {
        self.container
            .start()
            .map_err(|e| format!("Failed to start container: {}", e))
    }

    // TODO async
    // pub async fn rs_container_pause(self: &RsContainer) -> Result<(), String>
    // pub async fn rs_container_unpause(self: &RsContainer) -> Result<(), String>

    // TODO callbacks
    // pub fn stdout(&self, follow: bool) -> Box<dyn BufRead + Send>
    // pub fn stderr(&self, follow: bool) -> Box<dyn BufRead + Send>

    pub fn rs_container_stdout_to_vec(self: &RsContainer) -> Result<Vec<u8>, String> {
        self.container
            .stdout_to_vec()
            .map_err(|e| format!("Failed to get stdout: {}", e))
    }

    pub fn rs_container_stderr_to_vec(self: &RsContainer) -> Result<Vec<u8>, String> {
        self.container
            .stderr_to_vec()
            .map_err(|e| format!("Failed to get stderr: {}", e))
    }

    pub fn rs_container_is_running(self: &RsContainer) -> Result<bool, String> {
        self.container
            .is_running()
            .map_err(|e| format!("Failed to get is running: {}", e))
    }

    pub fn rs_container_exit_code_opt(self: &RsContainer) -> Result<Vec<i64>, String> {
        self.container
            .exit_code()
            .map_err(|e| format!("Failed to get exit code: {}", e))
            .map(|opt| opt.into_iter().collect())
    }

    // TODO async
    // fn rt(&self) -> &Arc<tokio::runtime::Runtime>
    // fn async_impl(&self) -> &ContainerAsync<I>
}

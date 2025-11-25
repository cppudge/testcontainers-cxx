use crate::{
    container::RsContainer, core::cgroupns_mode::RsCgroupnsMode,
    core::container_port::RsContainerPort, core::copy_data_source::RsCopyDataSource,
    core::healthcheck::RsHealthcheck, core::host::RsHost, core::mount::RsMount,
    core::wait::wait_for::RsWaitFor,
};
use std::time::Duration;
use testcontainers::{runners::SyncRunner, ContainerRequest, GenericImage, ImageExt};

pub struct RsContainerRequest {
    pub container: ContainerRequest<GenericImage>,
}

pub fn rs_container_request_destroy(container: Box<RsContainerRequest>) {
    drop(container);
}

pub fn rs_container_request_with_cmd(
    container_request: Box<RsContainerRequest>,
    cmd: Vec<String>,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_cmd(cmd),
    ))
}

pub fn rs_container_request_with_name(
    container_request: Box<RsContainerRequest>,
    name: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_name(name),
    ))
}

pub fn rs_container_request_with_tag(
    container_request: Box<RsContainerRequest>,
    tag: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_tag(tag),
    ))
}

pub fn rs_container_request_with_container_name(
    container_request: Box<RsContainerRequest>,
    name: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_container_name(name),
    ))
}

pub fn rs_container_request_with_platform(
    container_request: Box<RsContainerRequest>,
    platform: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_platform(platform),
    ))
}

pub fn rs_container_request_with_network(
    container_request: Box<RsContainerRequest>,
    network: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_network(network),
    ))
}

pub fn rs_container_request_with_label(
    container_request: Box<RsContainerRequest>,
    key: String,
    value: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_label(key, value),
    ))
}

pub fn rs_container_request_with_labels(
    container_request: Box<RsContainerRequest>,
    keys: Vec<String>,
    values: Vec<String>,
) -> Box<RsContainerRequest> {
    let rust_labels: Vec<(String, String)> = keys.into_iter().zip(values.into_iter()).collect();
    Box::new(RsContainerRequest::new(
        container_request.container.with_labels(rust_labels),
    ))
}

pub fn rs_container_request_with_host(
    container_request: Box<RsContainerRequest>,
    key: String,
    value: Box<RsHost>,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_host(key, *value),
    ))
}

pub fn rs_container_request_with_mount(
    container_request: Box<RsContainerRequest>,
    mount: Box<RsMount>,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_mount(*mount),
    ))
}

pub fn rs_container_request_with_env_var(
    container_request: Box<RsContainerRequest>,
    name: String,
    value: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_env_var(name, value),
    ))
}

pub fn rs_container_request_with_hostname(
    container_request: Box<RsContainerRequest>,
    hostname: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_hostname(hostname),
    ))
}

pub fn rs_container_request_with_mapped_port(
    container_request: Box<RsContainerRequest>,
    host_port: u16,
    container_port: Box<RsContainerPort>,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request
            .container
            .with_mapped_port(host_port, (*container_port).into()),
    ))
}

pub fn rs_container_request_with_privileged(
    container_request: Box<RsContainerRequest>,
    privileged: bool,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_privileged(privileged),
    ))
}

pub fn rs_container_request_with_cap_add(
    container_request: Box<RsContainerRequest>,
    capability: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_cap_add(capability),
    ))
}

pub fn rs_container_request_with_cap_drop(
    container_request: Box<RsContainerRequest>,
    capability: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_cap_drop(capability),
    ))
}

pub fn rs_container_request_with_cgroupns_mode(
    container_request: Box<RsContainerRequest>,
    mode: Box<RsCgroupnsMode>,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request
            .container
            .with_cgroupns_mode((*mode).into()),
    ))
}

pub fn rs_container_request_with_userns_mode(
    container_request: Box<RsContainerRequest>,
    userns_mode: &str,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_userns_mode(userns_mode),
    ))
}

pub fn rs_container_request_with_shm_size(
    container_request: Box<RsContainerRequest>,
    bytes: u64,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_shm_size(bytes),
    ))
}

pub fn rs_container_request_with_startup_timeout(
    container_request: Box<RsContainerRequest>,
    timeout_ns: u64,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request
            .container
            .with_startup_timeout(Duration::from_nanos(timeout_ns)),
    ))
}

pub fn rs_container_request_with_working_dir(
    container_request: Box<RsContainerRequest>,
    working_dir: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_working_dir(working_dir),
    ))
}

pub fn rs_container_request_with_user(
    container_request: Box<RsContainerRequest>,
    user: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_user(user),
    ))
}

pub fn rs_container_request_with_readonly_rootfs(
    container_request: Box<RsContainerRequest>,
    readonly_rootfs: bool,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request
            .container
            .with_readonly_rootfs(readonly_rootfs),
    ))
}

pub fn rs_container_request_with_security_opt(
    container_request: Box<RsContainerRequest>,
    security_opt: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_security_opt(security_opt),
    ))
}

pub fn rs_container_request_with_ready_conditions(
    container_request: Box<RsContainerRequest>,
    ready_conditions: Vec<RsWaitFor>,
) -> Box<RsContainerRequest> {
    let conditions: Vec<_> = ready_conditions.into_iter().map(|wf| wf.strategy).collect();
    Box::new(RsContainerRequest::new(
        container_request
            .container
            .with_ready_conditions(conditions),
    ))
}

pub fn rs_container_request_with_copy_to(
    container_request: Box<RsContainerRequest>,
    target: String,
    source: Box<RsCopyDataSource>,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request.container.with_copy_to(target, *source),
    ))
}

pub fn rs_container_request_with_ulimit(
    container_request: Box<RsContainerRequest>,
    name: &str,
    soft: i64,
    hard_opt: Vec<i64>,
) -> Box<RsContainerRequest> {
    let hard_opt = hard_opt.first().copied();
    Box::new(RsContainerRequest::new(
        container_request
            .container
            .with_ulimit(name, soft, hard_opt),
    ))
}

pub fn rs_container_request_with_health_check(
    container_request: Box<RsContainerRequest>,
    health_check: Box<RsHealthcheck>,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        container_request
            .container
            .with_health_check(health_check.healthcheck),
    ))
}

pub fn rs_container_request_start(
    container_request: Box<RsContainerRequest>,
) -> Result<Box<RsContainer>, String> {
    let container = container_request
        .container
        .start()
        .map_err(|e| format!("Failed to start container: {}", e))?;
    Ok(Box::new(RsContainer::new(container)))
}

pub fn rs_container_request_pull(
    container_request: Box<RsContainerRequest>,
) -> Result<Box<RsContainerRequest>, String> {
    let container_request = container_request
        .container
        .pull_image()
        .map_err(|e| format!("Failed to pull container: {}", e))?;
    Ok(Box::new(RsContainerRequest::new(container_request)))
}

impl RsContainerRequest {
    pub fn new(container: ContainerRequest<GenericImage>) -> Self {
        Self { container }
    }
}

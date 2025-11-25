use crate::{
    container::RsContainer, container_request::RsContainerRequest,
    core::cgroupns_mode::RsCgroupnsMode, core::container_port::RsContainerPort,
    core::copy_data_source::RsCopyDataSource, core::healthcheck::RsHealthcheck, core::host::RsHost,
    core::mount::RsMount, core::wait::wait_for::RsWaitFor,
};
use std::time::Duration;
use testcontainers::{runners::SyncRunner, GenericImage, Image, ImageExt};

pub struct RsGenericImage {
    image: GenericImage,
}

pub fn rs_generic_image_new(name: String, tag: String) -> Box<RsGenericImage> {
    Box::new(RsGenericImage::new(GenericImage::new(name, tag)))
}

pub fn rs_generic_image_destroy(image: Box<RsGenericImage>) {
    drop(image);
}

pub fn rs_generic_image_with_exposed_port(
    image: Box<RsGenericImage>,
    port: Box<RsContainerPort>,
) -> Box<RsGenericImage> {
    Box::new(RsGenericImage::new(
        image.image.with_exposed_port((*port).into()),
    ))
}

pub fn rs_generic_image_with_entrypoint(
    image: Box<RsGenericImage>,
    entrypoint: &str,
) -> Box<RsGenericImage> {
    Box::new(RsGenericImage::new(image.image.with_entrypoint(entrypoint)))
}

pub fn rs_generic_image_with_wait_for(
    image: Box<RsGenericImage>,
    wait_for: Box<RsWaitFor>,
) -> Box<RsGenericImage> {
    Box::new(RsGenericImage::new(
        image.image.with_wait_for(wait_for.strategy),
    ))
}

pub fn rs_generic_image_start(image: Box<RsGenericImage>) -> Result<Box<RsContainer>, String> {
    let container = image
        .image
        .start()
        .map_err(|e| format!("Failed to start container: {}", e))?;
    Ok(Box::new(RsContainer::new(container)))
}

pub fn rs_generic_image_pull(
    image: Box<RsGenericImage>,
) -> Result<Box<RsContainerRequest>, String> {
    let container_request = image
        .image
        .pull_image()
        .map_err(|e| format!("Failed to pull container: {}", e))?;
    Ok(Box::new(RsContainerRequest::new(container_request)))
}

pub fn rs_generic_image_with_cmd(
    image: Box<RsGenericImage>,
    cmd: Vec<String>,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(image.image.with_cmd(cmd)))
}

pub fn rs_generic_image_with_name(
    image: Box<RsGenericImage>,
    name: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(image.image.with_name(name)))
}

pub fn rs_generic_image_with_tag(
    image: Box<RsGenericImage>,
    tag: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(image.image.with_tag(tag)))
}

pub fn rs_generic_image_with_container_name(
    image: Box<RsGenericImage>,
    name: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        image.image.with_container_name(name),
    ))
}

pub fn rs_generic_image_with_platform(
    image: Box<RsGenericImage>,
    platform: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(image.image.with_platform(platform)))
}

pub fn rs_generic_image_with_network(
    image: Box<RsGenericImage>,
    network: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(image.image.with_network(network)))
}

pub fn rs_generic_image_with_label(
    image: Box<RsGenericImage>,
    key: String,
    value: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(image.image.with_label(key, value)))
}

pub fn rs_generic_image_with_labels(
    image: Box<RsGenericImage>,
    keys: Vec<String>,
    values: Vec<String>,
) -> Box<RsContainerRequest> {
    let rust_labels: Vec<(String, String)> = keys.into_iter().zip(values.into_iter()).collect();
    Box::new(RsContainerRequest::new(
        image.image.with_labels(rust_labels),
    ))
}

pub fn rs_generic_image_with_host(
    image: Box<RsGenericImage>,
    key: String,
    value: Box<RsHost>,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(image.image.with_host(key, *value)))
}

pub fn rs_generic_image_with_mount(
    image: Box<RsGenericImage>,
    mount: Box<RsMount>,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(image.image.with_mount(*mount)))
}

pub fn rs_generic_image_with_env_var(
    image: Box<RsGenericImage>,
    name: String,
    value: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        image.image.with_env_var(name, value),
    ))
}

pub fn rs_generic_image_with_hostname(
    image: Box<RsGenericImage>,
    hostname: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(image.image.with_hostname(hostname)))
}

pub fn rs_generic_image_with_mapped_port(
    image: Box<RsGenericImage>,
    host_port: u16,
    container_port: Box<RsContainerPort>,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        image
            .image
            .with_mapped_port(host_port, (*container_port).into()),
    ))
}

pub fn rs_generic_image_with_privileged(
    image: Box<RsGenericImage>,
    privileged: bool,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        image.image.with_privileged(privileged),
    ))
}

pub fn rs_generic_image_with_cap_add(
    image: Box<RsGenericImage>,
    capability: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        image.image.with_cap_add(capability),
    ))
}

pub fn rs_generic_image_with_cap_drop(
    image: Box<RsGenericImage>,
    capability: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        image.image.with_cap_drop(capability),
    ))
}

pub fn rs_generic_image_with_cgroupns_mode(
    image: Box<RsGenericImage>,
    mode: Box<RsCgroupnsMode>,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        image.image.with_cgroupns_mode((*mode).into()),
    ))
}

pub fn rs_generic_image_with_userns_mode(
    image: Box<RsGenericImage>,
    userns_mode: &str,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        image.image.with_userns_mode(&userns_mode),
    ))
}

pub fn rs_generic_image_with_shm_size(
    image: Box<RsGenericImage>,
    bytes: u64,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(image.image.with_shm_size(bytes)))
}

pub fn rs_generic_image_with_startup_timeout(
    image: Box<RsGenericImage>,
    timeout_ns: u64,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        image
            .image
            .with_startup_timeout(Duration::from_nanos(timeout_ns)),
    ))
}

pub fn rs_generic_image_with_working_dir(
    image: Box<RsGenericImage>,
    working_dir: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        image.image.with_working_dir(working_dir),
    ))
}

pub fn rs_generic_image_with_user(
    image: Box<RsGenericImage>,
    user: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(image.image.with_user(user)))
}

pub fn rs_generic_image_with_readonly_rootfs(
    image: Box<RsGenericImage>,
    readonly_rootfs: bool,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        image.image.with_readonly_rootfs(readonly_rootfs),
    ))
}

pub fn rs_generic_image_with_security_opt(
    image: Box<RsGenericImage>,
    security_opt: String,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        image.image.with_security_opt(security_opt),
    ))
}

pub fn rs_generic_image_with_ready_conditions(
    image: Box<RsGenericImage>,
    ready_conditions: Vec<RsWaitFor>,
) -> Box<RsContainerRequest> {
    let conditions: Vec<_> = ready_conditions.into_iter().map(|wf| wf.strategy).collect();
    Box::new(RsContainerRequest::new(
        image.image.with_ready_conditions(conditions),
    ))
}

pub fn rs_generic_image_with_copy_to(
    image: Box<RsGenericImage>,
    target: String,
    source: Box<RsCopyDataSource>,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        image.image.with_copy_to(target, *source),
    ))
}

pub fn rs_generic_image_with_ulimit(
    image: Box<RsGenericImage>,
    name: &str,
    soft: i64,
    hard_opt: Vec<i64>,
) -> Box<RsContainerRequest> {
    let hard_opt = hard_opt.first().copied();
    // WARNING: This will fail if hard_opt is nullopt because of bug in testcontainers-rs???
    Box::new(RsContainerRequest::new(
        image.image.with_ulimit(name, soft, hard_opt),
    ))
}

pub fn rs_generic_image_with_health_check(
    image: Box<RsGenericImage>,
    health_check: Box<RsHealthcheck>,
) -> Box<RsContainerRequest> {
    Box::new(RsContainerRequest::new(
        image.image.with_health_check(health_check.healthcheck),
    ))
}

impl RsGenericImage {
    pub fn new(image: GenericImage) -> Self {
        Self { image }
    }

    pub fn rs_generic_image_name(&self) -> &str {
        self.image.name()
    }

    pub fn rs_generic_image_tag(&self) -> &str {
        self.image.tag()
    }

    pub fn rs_generic_image_entrypoint_opt(&self) -> Vec<String> {
        self.image
            .entrypoint()
            .map(|s| s.to_string())
            .into_iter()
            .collect()
    }

    pub fn rs_generic_image_expose_ports(&self) -> Vec<RsContainerPort> {
        self.image
            .expose_ports()
            .iter()
            .map(|port| RsContainerPort::from(*port))
            .collect()
    }

    pub fn rs_generic_image_ready_conditions(&self) -> Vec<RsWaitFor> {
        self.image
            .ready_conditions()
            .into_iter()
            .map(|wf| RsWaitFor { strategy: wf })
            .collect()
    }
}

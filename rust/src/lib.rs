pub mod buildable_image;
pub mod container;
pub mod container_request;
pub mod core;
pub mod image;
pub mod system;

use crate::buildable_image::{
    rs_generic_buildable_image_build, rs_generic_buildable_image_destroy,
    rs_generic_buildable_image_new, rs_generic_buildable_image_with_data,
    rs_generic_buildable_image_with_dockerfile, rs_generic_buildable_image_with_dockerfile_string,
    rs_generic_buildable_image_with_file, RsGenericBuildableImage,
};
use crate::container::{rs_container_destroy, rs_container_rm, RsContainer};
use crate::container_request::{
    rs_container_request_destroy, rs_container_request_pull, rs_container_request_start,
    rs_container_request_with_cap_add, rs_container_request_with_cap_drop,
    rs_container_request_with_cgroupns_mode, rs_container_request_with_cmd,
    rs_container_request_with_container_name, rs_container_request_with_copy_to,
    rs_container_request_with_env_var, rs_container_request_with_health_check,
    rs_container_request_with_host, rs_container_request_with_hostname,
    rs_container_request_with_label, rs_container_request_with_labels,
    rs_container_request_with_mapped_port, rs_container_request_with_mount,
    rs_container_request_with_name, rs_container_request_with_network,
    rs_container_request_with_platform, rs_container_request_with_privileged,
    rs_container_request_with_readonly_rootfs, rs_container_request_with_ready_conditions,
    rs_container_request_with_security_opt, rs_container_request_with_shm_size,
    rs_container_request_with_startup_timeout, rs_container_request_with_tag,
    rs_container_request_with_ulimit, rs_container_request_with_user,
    rs_container_request_with_userns_mode, rs_container_request_with_working_dir,
    RsContainerRequest,
};
use crate::core::cgroupns_mode::{
    rs_cgroupns_mode_destroy, rs_cgroupns_mode_host, rs_cgroupns_mode_private, RsCgroupnsMode,
};
use crate::core::container_port::{
    rs_container_port_destroy, rs_container_port_sctp, rs_container_port_tcp,
    rs_container_port_udp, rs_container_port_vec_pop, RsContainerPort,
};
use crate::core::copy_data_source::{
    rs_copy_data_source_data, rs_copy_data_source_destroy, rs_copy_data_source_file,
    RsCopyDataSource,
};
use crate::core::exec::exec_command::{
    rs_exec_command_destroy, rs_exec_command_new, rs_exec_command_with_container_ready_conditions,
    RsExecCommand,
};
use crate::core::exec::sync_exec_result::{
    rs_sync_exec_result_destroy, rs_sync_exec_result_exit_code_opt,
    rs_sync_exec_result_stderr_to_vec, rs_sync_exec_result_stdout_to_vec, RsSyncExecResult,
};
use crate::core::healthcheck::{
    rs_healthcheck_cmd, rs_healthcheck_cmd_shell, rs_healthcheck_destroy, rs_healthcheck_empty,
    rs_healthcheck_none, rs_healthcheck_with_interval, rs_healthcheck_with_retries,
    rs_healthcheck_with_start_interval, rs_healthcheck_with_start_period,
    rs_healthcheck_with_timeout, RsHealthcheck,
};
use crate::core::host::{rs_host_addr, rs_host_destroy, rs_host_host_gateway_linux, RsHost};
use crate::core::mount::{
    rs_mount_bind_mount, rs_mount_destroy, rs_mount_tmpfs_mount, rs_mount_volume_mount,
    rs_mount_with_mode, rs_mount_with_read_only, rs_mount_with_read_write,
    rs_mount_with_size_bytes, RsMount,
};
use crate::core::wait::exit_wait_strategy::{
    rs_exit_wait_strategy_destroy, rs_exit_wait_strategy_new, rs_exit_wait_strategy_with_exit_code,
    rs_exit_wait_strategy_with_poll_interval, RsExitWaitStrategy,
};
use crate::core::wait::health_wait_strategy::{
    rs_health_wait_strategy_destroy, rs_health_wait_strategy_new,
    rs_health_wait_strategy_with_poll_interval, RsHealthWaitStrategy,
};
use crate::core::wait::log_wait_strategy::{
    rs_log_wait_strategy_destroy, rs_log_wait_strategy_stderr, rs_log_wait_strategy_stdout,
    rs_log_wait_strategy_stdout_or_stderr, rs_log_wait_strategy_with_times, RsLogWaitStrategy,
};
use crate::core::wait::wait_for::{
    rs_wait_for_destroy, rs_wait_for_duration, rs_wait_for_exit,
    rs_wait_for_healthcheck, rs_wait_for_log, rs_wait_for_millis_in_env_var, rs_wait_for_nothing,
    rs_wait_for_vec_pop, rs_wait_for_vec_push, RsWaitFor,
};
use crate::image::{
    rs_generic_image_destroy, rs_generic_image_new, rs_generic_image_pull, rs_generic_image_start,
    rs_generic_image_with_cap_add, rs_generic_image_with_cap_drop,
    rs_generic_image_with_cgroupns_mode, rs_generic_image_with_cmd,
    rs_generic_image_with_container_name, rs_generic_image_with_copy_to,
    rs_generic_image_with_entrypoint, rs_generic_image_with_env_var,
    rs_generic_image_with_exposed_port, rs_generic_image_with_health_check,
    rs_generic_image_with_host, rs_generic_image_with_hostname, rs_generic_image_with_label,
    rs_generic_image_with_labels, rs_generic_image_with_mapped_port, rs_generic_image_with_mount,
    rs_generic_image_with_name, rs_generic_image_with_network, rs_generic_image_with_platform,
    rs_generic_image_with_privileged, rs_generic_image_with_readonly_rootfs,
    rs_generic_image_with_ready_conditions, rs_generic_image_with_security_opt,
    rs_generic_image_with_shm_size, rs_generic_image_with_startup_timeout,
    rs_generic_image_with_tag, rs_generic_image_with_ulimit, rs_generic_image_with_user,
    rs_generic_image_with_userns_mode, rs_generic_image_with_wait_for,
    rs_generic_image_with_working_dir, RsGenericImage,
};
use crate::system::ip::ip_addr::{
    rs_ip_addr_destroy, rs_ip_addr_from_ipv4, rs_ip_addr_from_ipv6, RsIpAddr,
};
use crate::system::ip::ipv4_addr::{
    rs_ipv4_addr_broadcast, rs_ipv4_addr_destroy, rs_ipv4_addr_from_bits, rs_ipv4_addr_from_octets,
    rs_ipv4_addr_localhost, rs_ipv4_addr_new, rs_ipv4_addr_unspecified, RsIpv4Addr,
};
use crate::system::ip::ipv6_addr::{
    rs_ipv6_addr_destroy, rs_ipv6_addr_from_bits, rs_ipv6_addr_from_octets,
    rs_ipv6_addr_from_segments, rs_ipv6_addr_localhost, rs_ipv6_addr_new, rs_ipv6_addr_unspecified,
    RsIpv6Addr,
};
use crate::system::path::{rs_path_destroy, rs_path_from_bytes, rs_path_from_utf16, RsPath};
use crate::system::url_host::{
    rs_url_host_destroy, rs_url_host_domain, rs_url_host_from_ipv4, rs_url_host_from_ipv6,
    RsUrlHost,
};

pub fn version() -> String {
    "0.1.0".to_string()
}

#[rustfmt::skip]
#[cxx::bridge]
mod ffi {

    #[repr(u16)]
    enum RsContainerPortType {
        Tcp = 7,
        Udp = 8,
        Sctp = 9,
    }

    extern "Rust" {
        fn version() -> String;

        type RsGenericImage;
        type RsGenericBuildableImage;
        type RsContainer;
        type RsContainerRequest;
        type RsWaitFor;
        type RsLogWaitStrategy;
        type RsHealthWaitStrategy;
        type RsExitWaitStrategy;
        type RsContainerPort;
        type RsHost;
        type RsMount;
        type RsCopyDataSource;
        type RsHealthcheck;
        type RsCgroupnsMode;
        type RsExecCommand;
        type RsSyncExecResult;
        type RsIpAddr;
        type RsIpv4Addr;
        type RsIpv6Addr;
        type RsUrlHost;
        type RsPath;

        fn rs_generic_image_new(name: String, tag: String) -> Box<RsGenericImage>;
        fn rs_generic_image_destroy(image: Box<RsGenericImage>);
        fn rs_generic_image_name(self: &RsGenericImage) -> &str;
        fn rs_generic_image_tag(self: &RsGenericImage) -> &str;
        fn rs_generic_image_entrypoint_opt(self: &RsGenericImage) -> Vec<String>;
        fn rs_generic_image_expose_ports(self: &RsGenericImage) -> Vec<RsContainerPort>;
        fn rs_generic_image_ready_conditions(self: &RsGenericImage) -> Vec<RsWaitFor>;
        
        fn rs_generic_buildable_image_new(name: String, tag: String) -> Box<RsGenericBuildableImage>;
        fn rs_generic_buildable_image_with_dockerfile(image: Box<RsGenericBuildableImage>, source: Box<RsPath>) -> Box<RsGenericBuildableImage>;
        fn rs_generic_buildable_image_with_dockerfile_string(image: Box<RsGenericBuildableImage>, content: String) -> Box<RsGenericBuildableImage>;
        fn rs_generic_buildable_image_with_file(image: Box<RsGenericBuildableImage>, source: Box<RsPath>, target: String) -> Box<RsGenericBuildableImage>;
        fn rs_generic_buildable_image_with_data(image: Box<RsGenericBuildableImage>, data: Vec<u8>, target: String) -> Box<RsGenericBuildableImage>;
        fn rs_generic_buildable_image_build(image: Box<RsGenericBuildableImage>) -> Result<Box<RsGenericImage>>;
        fn rs_generic_buildable_image_destroy(image: Box<RsGenericBuildableImage>);
        fn rs_generic_image_with_exposed_port(image: Box<RsGenericImage>, port: Box<RsContainerPort>) -> Box<RsGenericImage>;
        fn rs_generic_image_with_entrypoint(image: Box<RsGenericImage>, entrypoint: &str) -> Box<RsGenericImage>;
        fn rs_generic_image_with_wait_for(image: Box<RsGenericImage>, wait_for: Box<RsWaitFor>) -> Box<RsGenericImage>;
        fn rs_generic_image_start(image: Box<RsGenericImage>) -> Result<Box<RsContainer>>;
        fn rs_generic_image_pull(image: Box<RsGenericImage>) -> Result<Box<RsContainerRequest>>;
        fn rs_generic_image_with_cmd(image: Box<RsGenericImage>, cmd: Vec<String>) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_name(image: Box<RsGenericImage>, name: String) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_tag(image: Box<RsGenericImage>, tag: String) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_container_name(image: Box<RsGenericImage>, name: String) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_platform(image: Box<RsGenericImage>, platform: String) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_network(image: Box<RsGenericImage>, network: String) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_label(image: Box<RsGenericImage>, key: String, value: String) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_labels(image: Box<RsGenericImage>, keys: Vec<String>, values: Vec<String>) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_host(image: Box<RsGenericImage>, key: String, value: Box<RsHost>) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_mount(image: Box<RsGenericImage>, mount: Box<RsMount>) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_env_var(image: Box<RsGenericImage>, name: String, value: String) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_hostname(image: Box<RsGenericImage>, hostname: String) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_mapped_port(image: Box<RsGenericImage>, host_port: u16, container_port: Box<RsContainerPort>) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_privileged(image: Box<RsGenericImage>, privileged: bool) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_cap_add(image: Box<RsGenericImage>, capability: String) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_cap_drop(image: Box<RsGenericImage>, capability: String) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_cgroupns_mode(image: Box<RsGenericImage>, mode: Box<RsCgroupnsMode>) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_userns_mode(image: Box<RsGenericImage>, userns_mode: &str) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_shm_size(image: Box<RsGenericImage>, bytes: u64) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_startup_timeout(image: Box<RsGenericImage>, timeout_ns: u64) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_working_dir(image: Box<RsGenericImage>, working_dir: String) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_user(image: Box<RsGenericImage>, user: String) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_readonly_rootfs(image: Box<RsGenericImage>, readonly_rootfs: bool) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_security_opt(image: Box<RsGenericImage>, security_opt: String) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_ready_conditions(image: Box<RsGenericImage>, ready_conditions: Vec<RsWaitFor>) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_copy_to(image: Box<RsGenericImage>, target: String, source: Box<RsCopyDataSource>) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_ulimit(image: Box<RsGenericImage>, name: &str, soft: i64, hard_opt: Vec<i64>) -> Box<RsContainerRequest>;
        fn rs_generic_image_with_health_check(image: Box<RsGenericImage>, health_check: Box<RsHealthcheck>) -> Box<RsContainerRequest>;

        fn rs_container_port_tcp(port: u16) -> Box<RsContainerPort>;
        fn rs_container_port_udp(port: u16) -> Box<RsContainerPort>;
        fn rs_container_port_sctp(port: u16) -> Box<RsContainerPort>;
        fn rs_container_port_destroy(port: Box<RsContainerPort>);
        fn rs_container_port_vec_pop(vec: &mut Vec<RsContainerPort>) -> Result<Box<RsContainerPort>>;
        fn rs_container_port_as_u16(self: &RsContainerPort) -> u16;
        fn rs_container_port_is_tcp(self: &RsContainerPort) -> bool;
        fn rs_container_port_is_udp(self: &RsContainerPort) -> bool;
        fn rs_container_port_is_sctp(self: &RsContainerPort) -> bool;

        fn rs_host_addr(addr: String) -> Result<Box<RsHost>>;
        fn rs_host_host_gateway_linux() -> Box<RsHost>;
        fn rs_host_destroy(host: Box<RsHost>);

        fn rs_mount_bind_mount(host_path: String, container_path: String) -> Box<RsMount>;
        fn rs_mount_volume_mount(name: String, container_path: String) -> Box<RsMount>;
        fn rs_mount_tmpfs_mount(container_path: String) -> Box<RsMount>;
        fn rs_mount_with_read_only(mount: Box<RsMount>) -> Box<RsMount>;
        fn rs_mount_with_read_write(mount: Box<RsMount>) -> Box<RsMount>;
        fn rs_mount_with_size_bytes(mount: Box<RsMount>, size: i64) -> Box<RsMount>;
        fn rs_mount_with_mode(mount: Box<RsMount>, mode: i64) -> Box<RsMount>;
        fn rs_mount_destroy(mount: Box<RsMount>);

        fn rs_copy_data_source_file(path: Box<RsPath>) -> Box<RsCopyDataSource>;
        fn rs_copy_data_source_data(data: Vec<u8>) -> Box<RsCopyDataSource>;
        fn rs_copy_data_source_destroy(source: Box<RsCopyDataSource>);

        fn rs_healthcheck_none() -> Box<RsHealthcheck>;
        fn rs_healthcheck_cmd_shell(command: String) -> Box<RsHealthcheck>;
        fn rs_healthcheck_cmd(command: Vec<String>) -> Box<RsHealthcheck>;
        fn rs_healthcheck_empty() -> Box<RsHealthcheck>;
        fn rs_healthcheck_with_interval(healthcheck: Box<RsHealthcheck>, interval_ns_opt: Vec<u64>) -> Box<RsHealthcheck>;
        fn rs_healthcheck_with_timeout(healthcheck: Box<RsHealthcheck>, timeout_ns_opt: Vec<u64>) -> Box<RsHealthcheck>;
        fn rs_healthcheck_with_retries(healthcheck: Box<RsHealthcheck>, retries_opt: Vec<u32>) -> Box<RsHealthcheck>;
        fn rs_healthcheck_with_start_period(healthcheck: Box<RsHealthcheck>, start_period_ns_opt: Vec<u64>) -> Box<RsHealthcheck>;
        fn rs_healthcheck_with_start_interval(healthcheck: Box<RsHealthcheck>, interval_ns_opt: Vec<u64>) -> Box<RsHealthcheck>;
        fn rs_healthcheck_destroy(healthcheck: Box<RsHealthcheck>);

        fn rs_container_destroy(container: Box<RsContainer>);
        fn rs_container_id(self: &RsContainer) -> &str;
        fn rs_container_image(self: &RsContainer) -> Box<RsGenericImage>;
        fn rs_container_get_host_port_ipv4(self: &RsContainer, port: Box<RsContainerPort>) -> Result<u16>;
        fn rs_container_get_host_port_ipv6(self: &RsContainer, port: Box<RsContainerPort>) -> Result<u16>;
        fn rs_container_get_bridge_ip_address(self: &RsContainer) -> Result<Box<RsIpAddr>>;
        fn rs_container_get_host(self: &RsContainer) -> Result<Box<RsUrlHost>>;
        fn rs_container_exec(self: &RsContainer, cmd: Box<RsExecCommand>) -> Result<Box<RsSyncExecResult>>;
        fn rs_container_stop(self: &RsContainer) -> Result<()>;
        fn rs_container_stop_with_timeout(self: &RsContainer, timeout_sec_opt: Vec<i32>) -> Result<()>;
        fn rs_container_start(self: &RsContainer) -> Result<()>;
        fn rs_container_rm(container: Box<RsContainer>) -> Result<()>;
        fn rs_container_stdout_to_vec(self: &RsContainer) -> Result<Vec<u8>>;
        fn rs_container_stderr_to_vec(self: &RsContainer) -> Result<Vec<u8>>;
        fn rs_container_is_running(self: &RsContainer) -> Result<bool>;
        fn rs_container_exit_code_opt(self: &RsContainer) -> Result<Vec<i64>>;

        fn rs_container_request_destroy(container_request: Box<RsContainerRequest>);
        fn rs_container_request_with_cmd(container_request: Box<RsContainerRequest>, cmd: Vec<String>) -> Box<RsContainerRequest>;
        fn rs_container_request_with_name(container_request: Box<RsContainerRequest>, name: String) -> Box<RsContainerRequest>;
        fn rs_container_request_with_tag(container_request: Box<RsContainerRequest>, tag: String) -> Box<RsContainerRequest>;
        fn rs_container_request_with_container_name(container_request: Box<RsContainerRequest>, name: String) -> Box<RsContainerRequest>;
        fn rs_container_request_with_platform(container_request: Box<RsContainerRequest>, platform: String) -> Box<RsContainerRequest>;
        fn rs_container_request_with_network(container_request: Box<RsContainerRequest>, network: String) -> Box<RsContainerRequest>;
        fn rs_container_request_with_label(container_request: Box<RsContainerRequest>, key: String, value: String) -> Box<RsContainerRequest>;
        fn rs_container_request_with_labels(container_request: Box<RsContainerRequest>, keys: Vec<String>, values: Vec<String>) -> Box<RsContainerRequest>;
        fn rs_container_request_with_host(container_request: Box<RsContainerRequest>, key: String, value: Box<RsHost>) -> Box<RsContainerRequest>;
        fn rs_container_request_with_mount(container_request: Box<RsContainerRequest>, mount: Box<RsMount>) -> Box<RsContainerRequest>;
        fn rs_container_request_with_env_var(container_request: Box<RsContainerRequest>, name: String, value: String) -> Box<RsContainerRequest>;
        fn rs_container_request_with_hostname(container_request: Box<RsContainerRequest>, hostname: String) -> Box<RsContainerRequest>;
        fn rs_container_request_with_mapped_port(container_request: Box<RsContainerRequest>, host_port: u16, container_port: Box<RsContainerPort>) -> Box<RsContainerRequest>;
        fn rs_container_request_with_privileged(container_request: Box<RsContainerRequest>, privileged: bool) -> Box<RsContainerRequest>;
        fn rs_container_request_with_cap_add(container_request: Box<RsContainerRequest>, capability: String) -> Box<RsContainerRequest>;
        fn rs_container_request_with_cap_drop(container_request: Box<RsContainerRequest>, capability: String) -> Box<RsContainerRequest>;
        fn rs_container_request_with_cgroupns_mode(container_request: Box<RsContainerRequest>, mode: Box<RsCgroupnsMode>) -> Box<RsContainerRequest>;
        fn rs_container_request_with_userns_mode(container_request: Box<RsContainerRequest>, userns_mode: &str) -> Box<RsContainerRequest>;
        fn rs_container_request_with_shm_size(container_request: Box<RsContainerRequest>, bytes: u64) -> Box<RsContainerRequest>;
        fn rs_container_request_with_startup_timeout(container_request: Box<RsContainerRequest>, timeout_ns: u64) -> Box<RsContainerRequest>;
        fn rs_container_request_with_working_dir(container_request: Box<RsContainerRequest>, working_dir: String) -> Box<RsContainerRequest>;
        fn rs_container_request_with_user(container_request: Box<RsContainerRequest>, user: String) -> Box<RsContainerRequest>;
        fn rs_container_request_with_readonly_rootfs(container_request: Box<RsContainerRequest>, readonly_rootfs: bool) -> Box<RsContainerRequest>;
        fn rs_container_request_with_security_opt(container_request: Box<RsContainerRequest>, security_opt: String) -> Box<RsContainerRequest>;
        fn rs_container_request_with_ready_conditions(container_request: Box<RsContainerRequest>, ready_conditions: Vec<RsWaitFor>) -> Box<RsContainerRequest>;
        fn rs_container_request_with_copy_to(container_request: Box<RsContainerRequest>, target: String, source: Box<RsCopyDataSource>) -> Box<RsContainerRequest>;
        fn rs_container_request_with_ulimit(container_request: Box<RsContainerRequest>, name: &str, soft: i64, hard_opt: Vec<i64>) -> Box<RsContainerRequest>;
        fn rs_container_request_with_health_check(container_request: Box<RsContainerRequest>, health_check: Box<RsHealthcheck>) -> Box<RsContainerRequest>;
        fn rs_container_request_start(container_request: Box<RsContainerRequest>) -> Result<Box<RsContainer>>;
        fn rs_container_request_pull(container_request: Box<RsContainerRequest>) -> Result<Box<RsContainerRequest>>;

        fn rs_wait_for_nothing() -> Box<RsWaitFor>;
        fn rs_wait_for_duration(duration_ns: u64) -> Box<RsWaitFor>;
        fn rs_wait_for_log(strategy: Box<RsLogWaitStrategy>) -> Box<RsWaitFor>;
        fn rs_wait_for_healthcheck(strategy: Box<RsHealthWaitStrategy>) -> Box<RsWaitFor>;
        fn rs_wait_for_exit(strategy: Box<RsExitWaitStrategy>) -> Box<RsWaitFor>;
        fn rs_wait_for_millis_in_env_var(name: &'static str) -> Box<RsWaitFor>;
        fn rs_wait_for_vec_push(vec: &mut Vec<RsWaitFor>, wait_for: Box<RsWaitFor>);
        fn rs_wait_for_vec_pop(vec: &mut Vec<RsWaitFor>) -> Result<Box<RsWaitFor>>;
        fn rs_wait_for_clone(self: &RsWaitFor) -> Box<RsWaitFor>;
        fn rs_wait_for_destroy(wait_for: Box<RsWaitFor>);

        fn rs_log_wait_strategy_destroy(strategy: Box<RsLogWaitStrategy>);
        fn rs_log_wait_strategy_stdout(message: String) -> Box<RsLogWaitStrategy>;
        fn rs_log_wait_strategy_stderr(message: String) -> Box<RsLogWaitStrategy>;
        fn rs_log_wait_strategy_stdout_or_stderr(message: String) -> Box<RsLogWaitStrategy>;
        fn rs_log_wait_strategy_with_times(strategy: Box<RsLogWaitStrategy>, times: usize) -> Box<RsLogWaitStrategy>;

        fn rs_health_wait_strategy_destroy(strategy: Box<RsHealthWaitStrategy>);
        fn rs_health_wait_strategy_new() -> Box<RsHealthWaitStrategy>;
        fn rs_health_wait_strategy_with_poll_interval(strategy: Box<RsHealthWaitStrategy>, interval_ns: u64) -> Box<RsHealthWaitStrategy>;

        fn rs_exit_wait_strategy_destroy(strategy: Box<RsExitWaitStrategy>);
        fn rs_exit_wait_strategy_new() -> Box<RsExitWaitStrategy>;
        fn rs_exit_wait_strategy_with_poll_interval(strategy: Box<RsExitWaitStrategy>, interval_ns: u64) -> Box<RsExitWaitStrategy>;
        fn rs_exit_wait_strategy_with_exit_code(strategy: Box<RsExitWaitStrategy>, exit_code: i64) -> Box<RsExitWaitStrategy>;

        fn rs_cgroupns_mode_private() -> Box<RsCgroupnsMode>;
        fn rs_cgroupns_mode_host() -> Box<RsCgroupnsMode>;
        fn rs_cgroupns_mode_destroy(mode: Box<RsCgroupnsMode>);

        fn rs_exec_command_new(cmd: Vec<String>) -> Box<RsExecCommand>;
        fn rs_exec_command_with_container_ready_conditions(command: Box<RsExecCommand>, ready_conditions: Vec<RsWaitFor>) -> Box<RsExecCommand>;
        fn rs_exec_command_destroy(command: Box<RsExecCommand>);

        fn rs_sync_exec_result_exit_code_opt(result: &mut RsSyncExecResult) -> Result<Vec<i64>>;
        fn rs_sync_exec_result_stdout_to_vec(result: &mut RsSyncExecResult) -> Result<Vec<u8>>;
        fn rs_sync_exec_result_stderr_to_vec(result: &mut RsSyncExecResult) -> Result<Vec<u8>>;
        fn rs_sync_exec_result_destroy(result: Box<RsSyncExecResult>);

        fn rs_ipv4_addr_new(a: u8, b: u8, c: u8, d: u8) -> Box<RsIpv4Addr>;
        fn rs_ipv4_addr_localhost() -> Box<RsIpv4Addr>;
        fn rs_ipv4_addr_unspecified() -> Box<RsIpv4Addr>;
        fn rs_ipv4_addr_broadcast() -> Box<RsIpv4Addr>;
        fn rs_ipv4_addr_from_bits(bits: u32) -> Box<RsIpv4Addr>;
        fn rs_ipv4_addr_from_octets(octets: [u8; 4]) -> Box<RsIpv4Addr>;
        fn rs_ipv4_addr_to_bits(self: &RsIpv4Addr) -> u32;
        fn rs_ipv4_addr_octets(self: &RsIpv4Addr) -> [u8; 4];
        fn rs_ipv4_addr_is_unspecified(self: &RsIpv4Addr) -> bool;
        fn rs_ipv4_addr_is_loopback(self: &RsIpv4Addr) -> bool;
        fn rs_ipv4_addr_is_private(self: &RsIpv4Addr) -> bool;
        fn rs_ipv4_addr_is_link_local(self: &RsIpv4Addr) -> bool;
        // fn rs_ipv4_addr_is_global(self: &RsIpv4Addr) -> bool;
        // fn rs_ipv4_addr_is_shared(self: &RsIpv4Addr) -> bool;
        // fn rs_ipv4_addr_is_benchmarking(self: &RsIpv4Addr) -> bool;
        // fn rs_ipv4_addr_is_reserved(self: &RsIpv4Addr) -> bool;
        fn rs_ipv4_addr_is_multicast(self: &RsIpv4Addr) -> bool;
        fn rs_ipv4_addr_is_broadcast(self: &RsIpv4Addr) -> bool;
        fn rs_ipv4_addr_is_documentation(self: &RsIpv4Addr) -> bool;
        fn rs_ipv4_addr_to_string(self: &RsIpv4Addr) -> String;
        fn rs_ipv4_addr_destroy(addr: Box<RsIpv4Addr>);

        fn rs_ipv6_addr_new(a: u16, b: u16, c: u16, d: u16, e: u16, f: u16, g: u16, h: u16) -> Box<RsIpv6Addr>;
        fn rs_ipv6_addr_localhost() -> Box<RsIpv6Addr>;
        fn rs_ipv6_addr_unspecified() -> Box<RsIpv6Addr>;
        fn rs_ipv6_addr_from_bits(bits_low: u64, bits_high: u64) -> Box<RsIpv6Addr>;
        fn rs_ipv6_addr_from_segments(segments: [u16; 8]) -> Box<RsIpv6Addr>;
        fn rs_ipv6_addr_from_octets(octets: [u8; 16]) -> Box<RsIpv6Addr>;
        fn rs_ipv6_addr_to_bits_pair(self: &RsIpv6Addr) -> [u64; 2];
        fn rs_ipv6_addr_segments(self: &RsIpv6Addr) -> [u16; 8];
        fn rs_ipv6_addr_octets(self: &RsIpv6Addr) -> [u8; 16];
        fn rs_ipv6_addr_is_unspecified(self: &RsIpv6Addr) -> bool;
        fn rs_ipv6_addr_is_loopback(self: &RsIpv6Addr) -> bool;
        // fn rs_ipv6_addr_is_global(self: &RsIpv6Addr) -> bool;
        // fn rs_ipv6_addr_is_unicast(self: &RsIpv6Addr) -> bool;
        fn rs_ipv6_addr_is_unicast_link_local(self: &RsIpv6Addr) -> bool;
        fn rs_ipv6_addr_is_unique_local(self: &RsIpv6Addr) -> bool;
        // fn rs_ipv6_addr_is_documentation(self: &RsIpv6Addr) -> bool;
        // fn rs_ipv6_addr_is_benchmarking(self: &RsIpv6Addr) -> bool;
        // fn rs_ipv6_addr_is_unicast_global(self: &RsIpv6Addr) -> bool;
        // fn rs_ipv6_addr_multicast_scope(self: &RsIpv6Addr) -> u8;
        fn rs_ipv6_addr_is_multicast(self: &RsIpv6Addr) -> bool;
        // fn rs_ipv6_addr_is_ipv4_mapped(self: &RsIpv6Addr) -> bool;
        fn rs_ipv6_addr_to_canonical(self: &RsIpv6Addr) -> Box<RsIpAddr>;
        fn rs_ipv6_addr_to_string(self: &RsIpv6Addr) -> String;
        fn rs_ipv6_addr_destroy(addr: Box<RsIpv6Addr>);

        fn rs_ip_addr_from_ipv4(ipv4: Box<RsIpv4Addr>) -> Box<RsIpAddr>;
        fn rs_ip_addr_from_ipv6(ipv6: Box<RsIpv6Addr>) -> Box<RsIpAddr>;
        fn rs_ip_addr_is_unspecified(self: &RsIpAddr) -> bool;
        fn rs_ip_addr_is_loopback(self: &RsIpAddr) -> bool;
        // fn rs_ip_addr_is_global(self: &RsIpAddr) -> bool;
        fn rs_ip_addr_is_multicast(self: &RsIpAddr) -> bool;
        // fn rs_ip_addr_is_documentation(self: &RsIpAddr) -> bool;
        // fn rs_ip_addr_is_benchmarking(self: &RsIpAddr) -> bool;
        fn rs_ip_addr_is_ipv4(self: &RsIpAddr) -> bool;
        fn rs_ip_addr_is_ipv6(self: &RsIpAddr) -> bool;
        fn rs_ip_addr_to_canonical(self: &RsIpAddr) -> Box<RsIpAddr>;
        fn rs_ip_addr_to_ipv4(self: &RsIpAddr) -> Result<Box<RsIpv4Addr>>;
        fn rs_ip_addr_to_ipv6(self: &RsIpAddr) -> Result<Box<RsIpv6Addr>>;
        fn rs_ip_addr_to_string(self: &RsIpAddr) -> String;
        fn rs_ip_addr_destroy(addr: Box<RsIpAddr>);

        fn rs_url_host_domain(domain: String) -> Box<RsUrlHost>;
        fn rs_url_host_from_ipv4(addr: Box<RsIpv4Addr>) -> Box<RsUrlHost>;
        fn rs_url_host_from_ipv6(addr: Box<RsIpv6Addr>) -> Box<RsUrlHost>;
        fn rs_url_host_is_domain(self: &RsUrlHost) -> bool;
        fn rs_url_host_is_ipv4(self: &RsUrlHost) -> bool;
        fn rs_url_host_is_ipv6(self: &RsUrlHost) -> bool;
        fn rs_url_host_to_domain(self: &RsUrlHost) -> Result<String>;
        fn rs_url_host_to_ipv4(self: &RsUrlHost) -> Result<Box<RsIpv4Addr>>;
        fn rs_url_host_to_ipv6(self: &RsUrlHost) -> Result<Box<RsIpv6Addr>>;
        fn rs_url_host_to_string(self: &RsUrlHost) -> String;
        fn rs_url_host_destroy(host: Box<RsUrlHost>);

        fn rs_path_from_bytes(b: &[u8]) -> Box<RsPath>;
        fn rs_path_from_utf16(w: &[u16]) -> Box<RsPath>;
        fn rs_path_to_string_opt(self: &RsPath) -> Result<Vec<String>>;
        fn rs_path_destroy(path: Box<RsPath>);
    }
}

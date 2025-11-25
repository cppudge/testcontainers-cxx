use testcontainers::core::Healthcheck;
use std::time::Duration;

pub struct RsHealthcheck {
    pub healthcheck: Healthcheck,
}

pub fn rs_healthcheck_none() -> Box<RsHealthcheck> {
    Box::new(RsHealthcheck::new(Healthcheck::none()))
}

pub fn rs_healthcheck_cmd_shell(command: String) -> Box<RsHealthcheck> {
    Box::new(RsHealthcheck::new(Healthcheck::cmd_shell(command)))
}

pub fn rs_healthcheck_cmd(command: Vec<String>) -> Box<RsHealthcheck> {
    Box::new(RsHealthcheck::new(Healthcheck::cmd(command)))
}

pub fn rs_healthcheck_empty() -> Box<RsHealthcheck> {
    Box::new(RsHealthcheck::new(Healthcheck::empty()))
}

pub fn rs_healthcheck_with_interval(healthcheck: Box<RsHealthcheck>, interval_ns_opt: Vec<u64>) -> Box<RsHealthcheck> {
    let interval = interval_ns_opt.first().map(|&ns| Duration::from_nanos(ns));
    Box::new(RsHealthcheck::new(healthcheck.healthcheck.with_interval(interval)))
}

pub fn rs_healthcheck_with_timeout(healthcheck: Box<RsHealthcheck>, timeout_ns_opt: Vec<u64>) -> Box<RsHealthcheck> {
    let timeout = timeout_ns_opt.first().map(|&ns| Duration::from_nanos(ns));
    Box::new(RsHealthcheck::new(healthcheck.healthcheck.with_timeout(timeout)))
}

pub fn rs_healthcheck_with_retries(healthcheck: Box<RsHealthcheck>, retries_opt: Vec<u32>) -> Box<RsHealthcheck> {
    let retries_opt = retries_opt.first().copied();
    Box::new(RsHealthcheck::new(healthcheck.healthcheck.with_retries(retries_opt)))
}

pub fn rs_healthcheck_with_start_period(healthcheck: Box<RsHealthcheck>, start_period_ns_opt: Vec<u64>) -> Box<RsHealthcheck> {
    let start_period = start_period_ns_opt.first().map(|&ns| Duration::from_nanos(ns));
    Box::new(RsHealthcheck::new(healthcheck.healthcheck.with_start_period(start_period)))
}

pub fn rs_healthcheck_with_start_interval(healthcheck: Box<RsHealthcheck>, interval_ns_opt: Vec<u64>) -> Box<RsHealthcheck> {
    let interval = interval_ns_opt.first().map(|&ns| Duration::from_nanos(ns));
    Box::new(RsHealthcheck::new(healthcheck.healthcheck.with_start_interval(interval)))
}

pub fn rs_healthcheck_destroy(healthcheck: Box<RsHealthcheck>) {
    drop(healthcheck);
}

impl RsHealthcheck {
    pub fn new(healthcheck: Healthcheck) -> Self {
        Self { healthcheck }
    }
}

impl Into<Healthcheck> for RsHealthcheck {
    fn into(self) -> Healthcheck {
        self.healthcheck
    }
}


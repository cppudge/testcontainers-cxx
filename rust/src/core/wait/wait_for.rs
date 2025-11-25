use crate::core::wait::{
    exit_wait_strategy::RsExitWaitStrategy, health_wait_strategy::RsHealthWaitStrategy,
    log_wait_strategy::RsLogWaitStrategy,
};
use std::time::Duration;
use testcontainers::core::WaitFor;

pub struct RsWaitFor {
    pub strategy: WaitFor,
}

pub fn rs_wait_for_nothing() -> Box<RsWaitFor> {
    Box::new(RsWaitFor::new(WaitFor::Nothing))
}

pub fn rs_wait_for_log(strategy: Box<RsLogWaitStrategy>) -> Box<RsWaitFor> {
    Box::new(RsWaitFor::new(WaitFor::Log(strategy.strategy)))
}

pub fn rs_wait_for_duration(duration_ns: u64) -> Box<RsWaitFor> {
    Box::new(RsWaitFor::new(WaitFor::Duration {
        length: Duration::from_nanos(duration_ns),
    }))
}

pub fn rs_wait_for_healthcheck(strategy: Box<RsHealthWaitStrategy>) -> Box<RsWaitFor> {
    Box::new(RsWaitFor::new(WaitFor::Healthcheck(strategy.strategy)))
}

pub fn rs_wait_for_exit(strategy: Box<RsExitWaitStrategy>) -> Box<RsWaitFor> {
    Box::new(RsWaitFor::new(WaitFor::Exit(strategy.strategy)))
}

pub fn rs_wait_for_millis_in_env_var(name: &'static str) -> Box<RsWaitFor> {
    Box::new(RsWaitFor::new(WaitFor::millis_in_env_var(name)))
}

// vec helpers
pub fn rs_wait_for_vec_push(vec: &mut Vec<RsWaitFor>, wait_for: Box<RsWaitFor>) {
    vec.push(*wait_for);
}

pub fn rs_wait_for_vec_pop(vec: &mut Vec<RsWaitFor>) -> Result<Box<RsWaitFor>, String> {
    vec.pop()
        .map(Box::new)
        .ok_or_else(|| "Vector is empty".to_string())
}

pub fn rs_wait_for_destroy(wait_for: Box<RsWaitFor>) {
    drop(wait_for);
}

impl RsWaitFor {
    pub fn new(strategy: WaitFor) -> Self {
        Self { strategy }
    }

    pub fn rs_wait_for_clone(self: &RsWaitFor) -> Box<RsWaitFor> {
        Box::new(RsWaitFor::new(self.strategy.clone()))
    }
}

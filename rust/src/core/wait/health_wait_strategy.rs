use testcontainers::core::wait::HealthWaitStrategy;
use std::time::Duration;

pub struct RsHealthWaitStrategy {
    pub strategy: HealthWaitStrategy,
}

pub fn rs_health_wait_strategy_destroy(strategy: Box<RsHealthWaitStrategy>) {
    drop(strategy);
}

pub fn rs_health_wait_strategy_new() -> Box<RsHealthWaitStrategy> {
    Box::new(RsHealthWaitStrategy::new(HealthWaitStrategy::default()))
}

pub fn rs_health_wait_strategy_with_poll_interval(
    mut strategy: Box<RsHealthWaitStrategy>,
    interval_ns: u64,
) -> Box<RsHealthWaitStrategy> {
    strategy.strategy = strategy.strategy.with_poll_interval(Duration::from_nanos(interval_ns));
    strategy
}

impl RsHealthWaitStrategy {
    pub fn new(strategy: HealthWaitStrategy) -> Self {
        Self { strategy }
    }
}


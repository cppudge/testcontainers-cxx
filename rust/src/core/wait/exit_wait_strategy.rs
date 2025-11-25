use testcontainers::core::wait::ExitWaitStrategy;
use std::time::Duration;

pub struct RsExitWaitStrategy {
    pub strategy: ExitWaitStrategy,
}

pub fn rs_exit_wait_strategy_destroy(strategy: Box<RsExitWaitStrategy>) {
    drop(strategy);
}

pub fn rs_exit_wait_strategy_new() -> Box<RsExitWaitStrategy> {
    Box::new(RsExitWaitStrategy::new(ExitWaitStrategy::new()))
}

pub fn rs_exit_wait_strategy_with_poll_interval(
    mut strategy: Box<RsExitWaitStrategy>,
    interval_ns: u64,
) -> Box<RsExitWaitStrategy> {
    strategy.strategy = strategy.strategy.with_poll_interval(Duration::from_nanos(interval_ns));
    strategy
}

pub fn rs_exit_wait_strategy_with_exit_code(
    mut strategy: Box<RsExitWaitStrategy>,
    exit_code: i64,
) -> Box<RsExitWaitStrategy> {
    strategy.strategy = strategy.strategy.with_exit_code(exit_code);
    strategy
}

impl RsExitWaitStrategy {
    pub fn new(strategy: ExitWaitStrategy) -> Self {
        Self { strategy }
    }
}


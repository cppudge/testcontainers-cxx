use testcontainers::core::wait::LogWaitStrategy;

/// Rust wrapper for LogWaitStrategy
pub struct RsLogWaitStrategy {
    pub strategy: LogWaitStrategy,
}

/// Destroy LogWaitStrategy instance
pub fn rs_log_wait_strategy_destroy(strategy: Box<RsLogWaitStrategy>) {
    drop(strategy);
}

/// Create LogWaitStrategy that waits for message in stdout
pub fn rs_log_wait_strategy_stdout(message: String) -> Box<RsLogWaitStrategy> {
    Box::new(RsLogWaitStrategy::new(LogWaitStrategy::stdout(message)))
}

/// Create LogWaitStrategy that waits for message in stderr
pub fn rs_log_wait_strategy_stderr(message: String) -> Box<RsLogWaitStrategy> {
    Box::new(RsLogWaitStrategy::new(LogWaitStrategy::stderr(message)))
}

/// Create LogWaitStrategy that waits for message in either stdout or stderr
pub fn rs_log_wait_strategy_stdout_or_stderr(message: String) -> Box<RsLogWaitStrategy> {
    Box::new(RsLogWaitStrategy::new(LogWaitStrategy::stdout_or_stderr(message)))
}

/// Set the number of times the message should appear
pub fn rs_log_wait_strategy_with_times(
    mut strategy: Box<RsLogWaitStrategy>,
    times: usize,
) -> Box<RsLogWaitStrategy> {
    strategy.strategy = strategy.strategy.clone().with_times(times);
    strategy
}

impl RsLogWaitStrategy {
    pub fn new(strategy: LogWaitStrategy) -> Self {
        Self { strategy }
    }
}


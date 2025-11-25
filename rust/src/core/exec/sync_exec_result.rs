use testcontainers::core::SyncExecResult;

pub struct RsSyncExecResult {
    pub result: SyncExecResult,
}

pub fn rs_sync_exec_result_exit_code_opt(result: &mut RsSyncExecResult) -> Result<Vec<i64>, String> {
    result
        .result
        .exit_code()
        .map_err(|e| format!("Failed to get exit code: {}", e))
        .map(|opt| opt.into_iter().collect())
}

pub fn rs_sync_exec_result_stdout_to_vec(result: &mut RsSyncExecResult) -> Result<Vec<u8>, String> {
    result
        .result
        .stdout_to_vec()
        .map_err(|e| format!("Failed to get stdout: {}", e))
}

pub fn rs_sync_exec_result_stderr_to_vec(result: &mut RsSyncExecResult) -> Result<Vec<u8>, String> {
    result
        .result
        .stderr_to_vec()
        .map_err(|e| format!("Failed to get stderr: {}", e))
}

pub fn rs_sync_exec_result_destroy(result: Box<RsSyncExecResult>) {
    drop(result);
}

impl RsSyncExecResult {
    pub fn new(result: SyncExecResult) -> Self {
        Self { result }
    }
}

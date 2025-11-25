use testcontainers::core::ExecCommand;
use crate::core::wait::wait_for::RsWaitFor;

pub struct RsExecCommand {
    pub command: ExecCommand,
}

pub fn rs_exec_command_new(cmd: Vec<String>) -> Box<RsExecCommand> {
    Box::new(RsExecCommand::new(ExecCommand::new(cmd)))
}

pub fn rs_exec_command_with_container_ready_conditions(
    command: Box<RsExecCommand>,
    ready_conditions: Vec<RsWaitFor>,
) -> Box<RsExecCommand> {
    let conditions: Vec<_> = ready_conditions.into_iter().map(|wf| wf.strategy).collect();
    Box::new(RsExecCommand::new(
        command.command.with_container_ready_conditions(conditions),
    ))
}

pub fn rs_exec_command_destroy(command: Box<RsExecCommand>) {
    drop(command);
}

impl RsExecCommand {
    pub fn new(command: ExecCommand) -> Self {
        Self { command }
    }
}


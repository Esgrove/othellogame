static VERSION_STRING: &str = concat!(
    "Othello Rust ",
    env!("VERSION"),
    " ",
    env!("BUILD_TIME"),
    " ",
    env!("GIT_BRANCH"),
    " ",
    env!("GIT_COMMIT")
);

/// Return version info string
pub fn version_info() -> &'static str {
    VERSION_STRING
}

//! # Version
//! Version information.
//!
//! Akseli Lukkarila
//! 2019-2026

// Build metadata set in `build.rs` at build time.

#[expect(dead_code)]
static BUILD_TIME: &str = env!("BUILD_TIME");
#[expect(dead_code)]
static GIT_BRANCH: &str = env!("GIT_BRANCH");
#[expect(dead_code)]
static GIT_COMMIT: &str = env!("GIT_COMMIT");
#[expect(dead_code)]
static VERSION_NUMBER: &str = env!("VERSION_NUMBER");

/// Full version string composed in `build.rs` at build time.
static VERSION_STRING: &str = env!("VERSION_STRING");

/// Return version info string
#[inline]
pub fn version_info() -> &'static str {
    VERSION_STRING
}

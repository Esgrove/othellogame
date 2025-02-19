use std::sync::LazyLock;

use shadow_rs::shadow;

// Get compile-time build information
shadow!(build);

static VERSION_STRING: LazyLock<String> = LazyLock::new(|| {
    format!(
        "Othello Rust {} {} {} {}",
        build::PKG_VERSION,
        build::BUILD_TIME,
        build::BRANCH,
        build::SHORT_COMMIT
    )
});

pub fn version_info() -> &'static str {
    &VERSION_STRING
}

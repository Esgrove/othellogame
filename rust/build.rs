use std::process::Command;

fn main() {
    let git_hash = Command::new("git")
        .args(["rev-parse", "--short", "HEAD"])
        .output()
        .map_or_else(
            |_| "unknown".to_string(),
            |o| String::from_utf8_lossy(&o.stdout).trim().to_string(),
        );

    let git_branch = Command::new("git")
        .args(["rev-parse", "--abbrev-ref", "HEAD"])
        .output()
        .map_or_else(
            |_| "unknown".to_string(),
            |o| String::from_utf8_lossy(&o.stdout).trim().to_string(),
        );

    let build_timestamp = Command::new("date")
        .args(["+%Y-%m-%d_%H%M"])
        .output()
        .ok()
        .and_then(|o| String::from_utf8(o.stdout).ok())
        .map_or_else(|| "unknown".to_string(), |s| s.trim().to_string());

    let version = std::env::var("CARGO_PKG_VERSION").unwrap_or_else(|_| "unknown".to_string());

    // Set compile time env variables
    println!("cargo:rustc-env=GIT_COMMIT={git_hash}");
    println!("cargo:rustc-env=GIT_BRANCH={git_branch}");
    println!("cargo:rustc-env=BUILD_TIMESTAMP={build_timestamp}");
    println!("cargo:rustc-env=VERSION={version}");

    // Tell Cargo to rerun if these change
    println!("cargo:rerun-if-changed=build.rs");
}

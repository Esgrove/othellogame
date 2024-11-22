//! # Main
//! Play Othello (Reversi) on the command line
//! <https://en.wikipedia.org/wiki/Reversi>
//!
//! Akseli Lukkarila
//! 2019-2024

mod board;
mod colorprint;
mod othello;
mod player;
mod utils;

use std::env;

use anyhow::Result;
use clap::{arg, Parser};
use colored::Colorize;
use shadow_rs::shadow;

use crate::othello::Othello;
use crate::utils::{Settings, DEFAULT_BOARD_SIZE, MAX_BOARD_SIZE, MIN_BOARD_SIZE};

// Get build information
shadow!(build);

/// Command line arguments
///
/// Basic info is read from `Cargo.toml`
/// See Clap `Derive` documentation for details:
/// <https://docs.rs/clap/latest/clap/_derive/index.html>
#[derive(Parser)]
#[command(
    author,
    about,
    arg_required_else_help = false,
    disable_version_flag = true
)]
struct Args {
    /// Optional board size (4..10)
    size: Option<usize>,

    /// Enable autoplay mode with both players controlled by computer
    #[arg(short, long, conflicts_with = "default")]
    autoplay: bool,

    /// Only print hash to check the result (implies `autoplay` and `test`)
    #[arg(short, long)]
    check: bool,

    /// Play with default settings
    #[arg(short, long)]
    default: bool,

    /// Show game log at the end
    #[arg(short, long)]
    log: bool,

    /// Hide disk placement hints
    #[arg(short, long)]
    no_helpers: bool,

    /// Enable test mode with deterministic computer moves
    #[arg(short, long)]
    test: bool,

    /// Custom version flag instead of clap default
    #[arg(short, long, help = "Print version and exit")]
    version: bool,
}

fn main() -> Result<()> {
    // Set to 1 to display backtraces for panics for debugging
    env::set_var("RUST_BACKTRACE", "0");

    // Parse command line arguments using clap
    let args = Args::parse();

    if args.version {
        println!(
            "Othello Rust {} {} {} {}",
            build::PKG_VERSION,
            build::BUILD_TIME,
            build::BRANCH,
            build::SHORT_COMMIT,
        );
        return Ok(());
    }

    println!("{}", "OTHELLO GAME - RUST".green().bold());

    let board_size: usize = {
        // Try to read board size from command line args
        if let Some(size) = args.size {
            if !(MIN_BOARD_SIZE..=MAX_BOARD_SIZE).contains(&size) {
                anyhow::bail!("Unsupported board size: {}", size)
            }
            println!("Using board size: {size}");
            size
        } else if args.autoplay || args.default {
            DEFAULT_BOARD_SIZE
        } else {
            // Otherwise ask user for board size
            Othello::get_board_size()
        }
    };

    let settings = Settings {
        board_size,
        autoplay_mode: args.autoplay || args.check,
        check_mode: args.check,
        show_helpers: !args.no_helpers,
        show_log: args.log,
        test_mode: args.test || args.check,
        use_defaults: args.default,
    };

    Othello::new(settings).play();
    Ok(())
}

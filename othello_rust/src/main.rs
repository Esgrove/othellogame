//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2023
//==========================================================

extern crate colored;
extern crate log;

use anyhow::Result;
use clap::{arg, Parser};
use colored::Colorize;
use shadow_rs::shadow;

use std::env;

use crate::othello::Othello;
use crate::utils::{Settings, DEFAULT_BOARD_SIZE, MAX_BOARD_SIZE, MIN_BOARD_SIZE};

mod board;
mod colorprint;
mod othello;
mod player;
mod utils;

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
    about = "A simple Othello CLI game implementation.",
    long_about = "A simple Othello CLI game implementation.",
    arg_required_else_help = false,
    disable_version_flag = true
)]
struct Args {
    /// Optional board size
    size: Option<usize>,

    /// Enable autoplay mode with both player controlled by computer
    #[arg(short, long, help = "Enable autoplay mode", conflicts_with = "default")]
    autoplay: bool,

    /// Only print hash to check result (implies `autoplay` and `test`)
    #[arg(short, long)]
    check: bool,

    /// Quick start to play with default settings
    #[arg(short, long, help = "Play with default settings")]
    default: bool,

    /// Show game log at the end of a game
    #[arg(short, long, help = "Show log after a game")]
    log: bool,

    /// Hide disk placement hints for human players
    #[arg(short, long, help = "Hide disk placement hints")]
    no_helpers: bool,

    /// Enable testing mode with deterministic computer move selection
    #[arg(short, long, help = "Enable test mode")]
    test: bool,

    /// Custom version flag instead of clap default
    #[arg(short, long, help = "Print version and exit")]
    version: bool,
}

fn main() -> Result<()> {
    // Uncomment to display backtrace in case of a panic
    env::set_var("RUST_BACKTRACE", "1");
    env_logger::init();

    println!("{}", "OTHELLO GAME - RUST".green().bold());

    // Parse command line arguments using clap
    let args = Args::parse();
    // Without clap:
    // let args: Vec<String> = env::args().collect();

    if args.version {
        println!(
            "Othello Rust {} {} {} {} {} {}",
            build::PKG_VERSION,
            build::BUILD_TIME,
            build::BRANCH,
            build::SHORT_COMMIT,
            build::BUILD_OS,
            build::RUST_VERSION,
        );
        return Ok(());
    }

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
        check_mode: args.check,
        test_mode: args.test || args.check,
        autoplay_mode: args.autoplay || args.check,
        use_defaults: args.default,
        show_helpers: !args.no_helpers,
        show_log: args.log,
    };

    let mut game = Othello::init(settings);
    game.play();
    Ok(())
}

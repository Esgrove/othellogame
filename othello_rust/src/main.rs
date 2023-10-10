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
use clap::Parser;
use colored::Colorize;
use shadow_rs::shadow;

use std::env;

use crate::othello::Othello;
use crate::utils::{MAX_BOARD_SIZE, MIN_BOARD_SIZE};

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
    /// Optional Othello board size
    size: Option<usize>,

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
    // let args: Vec<String> = env::args().collect();

    if args.version {
        println!(
            "{} {} {} {} {}",
            build::PROJECT_NAME,
            build::PKG_VERSION,
            build::BUILD_TIME,
            build::SHORT_COMMIT,
            build::BRANCH,
        );
        println!("{} {}", build::BUILD_OS, build::RUST_VERSION);
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
        } else {
            // Otherwise ask user for board size
            Othello::get_board_size()
        }
    };

    let mut game = Othello::init(board_size);
    game.play();
    Ok(())
}

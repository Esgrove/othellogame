//! # Othello
//! Play Othello (Reversi) on the command line
//! <https://en.wikipedia.org/wiki/Reversi>
//!
//! Akseli Lukkarila
//! 2019-2025

use std::io::{self, Write};

use colored::Colorize;

use crate::board::Board;
use crate::colorprint::print_warn;
use crate::player::Player;
use crate::settings::Settings;
use crate::types::Disk;
use crate::utils;
use crate::utils::{DEFAULT_BOARD_SIZE, MAX_BOARD_SIZE, MIN_BOARD_SIZE};

/// Gameplay loop and main logic.
pub struct Othello {
    board: Board,
    settings: Settings,
    player_black: Player,
    player_white: Player,
    game_log: Vec<String>,
    games_played: usize,
    rounds_played: usize,
}

impl Othello {
    /// Initialize Othello game.
    pub fn new(settings: Settings) -> Self {
        Self {
            board: Board::new(settings.board_size),
            settings,
            player_black: Player::black(settings.to_player_settings()),
            player_white: Player::white(settings.to_player_settings()),
            game_log: Vec::new(),
            rounds_played: 0,
            games_played: 0,
        }
    }

    /// Play one full game of Othello.
    pub fn play(&mut self) {
        loop {
            self.init_game();
            self.game_loop();
            self.print_result();
            if self.settings.show_log {
                self.print_log();
            }
            if self.settings.autoplay_mode
                || !Self::get_answer("Would you like to play again", "y", "n")
            {
                break;
            }
        }
    }

    /// Initialize game board and players for a new game.
    fn init_game(&mut self) {
        // Re-use existing objects instead of initializing new ones
        if self.games_played > 0 {
            self.board = Board::new(self.settings.board_size);
            self.player_black.reset();
            self.player_white.reset();
            self.rounds_played = 0;
            self.game_log.clear();
        }
        if self.settings.autoplay_mode {
            // Computer plays both
            self.player_white.set_computer();
            self.player_black.set_computer();
        } else if !self.settings.use_defaults
            && Self::get_answer("Would you like to play against the computer", "y", "n")
        {
            if Self::get_answer("Would you like to play as black or white", "b", "w") {
                self.player_white.set_computer();
            } else {
                self.player_black.set_computer();
            }
        }
        if !self.settings.check_mode {
            println!("{}", "\nPlayers:".bold());
            self.print_status();
        }
    }

    /// Keep making moves until both players can't make a move any more.
    fn game_loop(&mut self) {
        while self.board.can_play() && (self.player_black.can_play || self.player_white.can_play) {
            self.rounds_played += 1;
            self.print_round_header();
            for player in &mut [&mut self.player_black, &mut self.player_white] {
                if let Some(result) = player.play_one_move(&mut self.board) {
                    self.game_log
                        .push(format!("{};{}", result, self.board.log_entry()));
                }
                if !self.settings.check_mode {
                    println!("--------------------------------");
                }
            }
        }
        self.games_played += 1;
        self.print_game_end_footer();
    }

    fn format_game_log(&self) -> String {
        self.game_log
            .iter()
            .enumerate()
            .map(|(index, line)| format!("{:02}: {}", index + 1, line))
            .collect::<Vec<_>>()
            .join("\n")
    }

    fn print_round_header(&self) {
        if !self.settings.check_mode {
            println!(
                "{}",
                format!("\n=========== ROUND: {} ===========", self.rounds_played).bold()
            );
        }
    }

    fn print_game_end_footer(&self) {
        if !self.settings.check_mode {
            println!("{}", "\n================================".bold());
            println!("{}", "The game is finished!\n".green().bold());
        }
    }

    /// Print game log which shows all moves made and the game board state after each move.
    fn print_log(&self) {
        let formatted_log = self.format_game_log();
        if !self.settings.check_mode {
            println!("{}", "Game log:".yellow().bold());
            println!("{formatted_log}");
        }
        let hex_hash = utils::calculate_sha256(&formatted_log);
        println!("{hex_hash}");
    }

    /// Print ending status and winner info.
    fn print_result(&self) {
        println!("{}", "Result:".bold());
        self.print_status();
        println!();

        let winner = self.board.result();
        if winner == Disk::Empty {
            println!("The game ended in a tie...\n");
        } else {
            println!("The winner is {winner}!\n");
        }
    }

    /// Print current board and player info.
    fn print_status(&self) {
        println!("{}", self.player_black);
        println!("{}\n", self.player_white);
        println!("{}", self.board);
    }

    /// Ask a question with two options, and return bool from user answer.
    // Associated aka static function (no self parameter).
    fn get_answer(question: &str, yes: &str, no: &str) -> bool {
        print!("{question} ({yes}/{no})? ");
        io::stdout().flush().expect("Failed to flush stdout.");

        let mut input = String::new();
        io::stdin().read_line(&mut input).expect("Input failed");
        input.trim().to_lowercase().as_str() == yes
    }

    /// Ask and return the desired board size.
    pub fn get_board_size() -> usize {
        print!("Choose board size (default is {DEFAULT_BOARD_SIZE}): ");
        let mut input = String::new();
        if io::stdout().flush().is_ok() && io::stdin().read_line(&mut input).is_ok() {
            if let Ok(board_size) = input.trim().parse::<usize>() {
                if !(MIN_BOARD_SIZE..=MAX_BOARD_SIZE).contains(&board_size) {
                    println!(
                        "{}",
                        format!(
                            "Limiting board size to valid range {MIN_BOARD_SIZE}..{MAX_BOARD_SIZE}"
                        )
                        .yellow()
                    );
                }
                return board_size.clamp(MIN_BOARD_SIZE, MAX_BOARD_SIZE);
            }
        }
        print_warn(format!("Invalid size, defaulting to {DEFAULT_BOARD_SIZE}...").as_str());
        DEFAULT_BOARD_SIZE
    }
}

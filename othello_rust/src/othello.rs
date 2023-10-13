//==========================================================
// Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2023
//==========================================================

extern crate colored;

use std::collections::HashMap;

use std::fmt::Write as FmtWrite;
use std::io::{self, Write as IoWrite};

use colored::Colorize;
use sha2::{Digest, Sha256};

use crate::board::Board;
use crate::colorprint::print_warn;
use crate::player::Player;
use crate::utils::{Disk, Settings, DEFAULT_BOARD_SIZE, MAX_BOARD_SIZE, MIN_BOARD_SIZE};

/// Gameplay loop and main logic.
pub struct Othello {
    board: Board,
    settings: Settings,
    games_played: u32,
    player_black: Player,
    player_white: Player,
    rounds_played: u32,
    game_log: HashMap<u32, Vec<String>>,
}

impl Othello {
    /// Initialize Othello game.
    // Typically this would be called `new` but using `init` to match other implementations
    pub fn init(settings: Settings) -> Othello {
        Othello {
            board: Board::new(settings.board_size),
            settings,
            player_black: Player::black(settings.to_player_settings()),
            player_white: Player::white(settings.to_player_settings()),
            rounds_played: 0,
            games_played: 0,
            game_log: HashMap::new(),
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
                || !Othello::get_answer("Would you like to play again", "y", "n")
            {
                break;
            }
        }
    }

    /// Initialize game board and players for a new game.
    fn init_game(&mut self) {
        if self.games_played > 0 {
            self.board = Board::new(self.settings.board_size);
            self.player_black.reset();
            self.player_white.reset();
            self.rounds_played = 0;
        }

        self.game_log.insert(self.games_played, Vec::new());

        if self.settings.quick_start {
            // Default: play as black against white computer player
            self.player_white.set_human(false);
        } else if self.settings.autoplay_mode {
            // Computer plays both
            self.player_white.set_human(false);
            self.player_black.set_human(false);
        } else if Othello::get_answer("Would you like to play against the computer", "y", "n") {
            if Othello::get_answer("Would you like to play as black or white", "b", "w") {
                self.player_white.set_human(false);
            } else {
                self.player_black.set_human(false);
            }
        }

        println!("{}", "\nPlayers:".bold());
        self.print_status();
    }

    /// Keep making moves until both players can't make a move any more.
    fn game_loop(&mut self) {
        while self.board.can_play() && (self.player_black.can_play || self.player_white.can_play) {
            self.rounds_played += 1;
            println!(
                "{}",
                format!("\n=========== ROUND: {} ===========", self.rounds_played).bold()
            );
            for player in [&mut self.player_black, &mut self.player_white].iter_mut() {
                if let Some(result) = player.play_one_move(&mut self.board) {
                    self.game_log
                        .entry(self.games_played)
                        .or_default()
                        .push(format!("{};{}", result, self.board.to_log_status()));
                }
                println!("--------------------------------");
            }
        }
        self.games_played += 1;
    }

    fn print_log(&self) {
        let log = self.game_log.get(&(self.games_played - 1)).unwrap();
        let formatted_log: String =
            log.iter()
                .enumerate()
                .fold(String::new(), |mut output, (index, line)| {
                    let _ = writeln!(output, "{:02}{}", index + 1, line);
                    output
                });
        let hash = Sha256::digest(formatted_log.as_bytes());
        let hex_hash = base16ct::lower::encode_string(&hash);

        println!("{}", "Game log:".bold());
        println!("{}", formatted_log.trim());
        println!("{}", hex_hash);
    }

    /// Print ending status and winner info.
    fn print_result(&self) {
        println!("{}", "\n================================".bold());
        println!("{}", "The game is finished!\n".green().bold());
        println!("{}", "Result:".bold());
        self.print_status();
        println!();

        let winner = self.board.result();
        if winner == Disk::Empty {
            println!("The game ended in a tie...\n");
        } else {
            println!("The winner is {}!\n", winner);
        }
    }

    /// Print current board and player info.
    fn print_status(&self) {
        println!("{}", self.player_black);
        println!("{}\n", self.player_white);
        println!("{}", self.board);
    }

    /// Ask a question with two options, and return bool from user answer.
    // Associated aka static function (no self parameter)
    fn get_answer(question: &str, yes: &str, no: &str) -> bool {
        print!("{} ({}/{})? ", question, yes, no);
        let mut input = String::new();
        io::stdout().flush().unwrap();
        io::stdin().read_line(&mut input).expect("Input failed");
        return input.trim().to_lowercase() == *yes;
    }

    /// Ask and return the desired board size.
    pub fn get_board_size() -> usize {
        print!("Choose board size (default is {}): ", DEFAULT_BOARD_SIZE);
        let mut input = String::new();
        if io::stdout().flush().is_ok() && io::stdin().read_line(&mut input).is_ok() {
            if let Ok(board_size) = input.trim().parse::<usize>() {
                if !(MIN_BOARD_SIZE..=MAX_BOARD_SIZE).contains(&board_size) {
                    println!("{}", format!("Limiting board size to valid range {MIN_BOARD_SIZE}...{MAX_BOARD_SIZE}").yellow());
                }
                return board_size.clamp(MIN_BOARD_SIZE, MAX_BOARD_SIZE);
            }
        }
        print_warn(format!("Invalid size, defaulting to {}...", DEFAULT_BOARD_SIZE).as_str());
        DEFAULT_BOARD_SIZE
    }
}

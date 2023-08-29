//==========================================================
// Othello
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2023
//==========================================================

extern crate colored;
use crate::colored::Colorize;

use std::io::{self, Write};

use crate::board::Board;
use crate::colorprint::{print_error, print_warn};
use crate::player::Player;
use crate::utils::Disk;
use crate::utils::{MAX_BOARD_SIZE, MIN_BOARD_SIZE};

/// Gameplay loop and main logic.
pub(crate) struct Othello {
    board_size: usize,
    board: Board,
    games_played: u32,
    player_black: Player,
    player_white: Player,
    rounds_played: u32,
}

impl Othello {
    /// Initialize Othello game.
    // Typically this would be `new` but using init to match other implementations
    pub fn init(size: usize) -> Othello {
        Othello {
            board: Board::new(size),
            board_size: size,
            player_black: Player::black(),
            player_white: Player::white(),
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
            if !Othello::get_answer("\nWould you like to play again", "y", "n") {
                break;
            }
        }
    }

    /// Initialize game board and players for a new game.
    fn init_game(&mut self) {
        if self.games_played > 0 {
            self.board = Board::new(self.board_size);
            self.player_black.reset();
            self.player_white.reset();
            self.rounds_played = 0;
        }

        if Othello::get_answer("Would you like to play against the computer", "y", "n") {
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
            self.player_black.play_one_move(&mut self.board);
            println!("--------------------------------");
            self.player_white.play_one_move(&mut self.board);
        }
    }

    /// Print ending status and winner info.
    fn print_result(&self) {
        println!("{}", "\n================================".bold());
        println!("{}", "The game is finished!\n".green());
        println!("{}", "Result:".bold());
        self.print_status();
        println!();

        let winner = self.board.result();
        if winner == Disk::Empty {
            println!("The game ended in a tie...");
        } else {
            println!("The winner is {}!", winner);
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
    fn get_answer(text: &str, yes: &str, no: &str) -> bool {
        print!("{} ({}/{})? ", text, yes, no);
        let mut input = String::new();
        io::stdout().flush().unwrap();
        io::stdin().read_line(&mut input).expect("Input failed");
        return input.trim().to_lowercase() == *yes;
    }

    /// Ask and return the desired board size.
    pub(crate) fn get_board_size() -> usize {
        print!("Choose board size (default is 8): ");
        let mut input = String::new();
        if io::stdout().flush().is_ok() && io::stdin().read_line(&mut input).is_ok() {
            if let Ok(board_size) = input.trim().parse::<usize>() {
                return board_size.clamp(MIN_BOARD_SIZE, MAX_BOARD_SIZE);
            }
        }
        print_warn("Invalid value, defaulting to 8...");
        return 8;
    }
}

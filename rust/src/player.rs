//! # Player
//! Defines one player for Othello
//!
//! Akseli Lukkarila
//! 2019-2025

use std::io::Write;
use std::{fmt, io};
use std::{thread, time::Duration};

use colored::Colorize;
use rand::{self, Rng};

use crate::board::Board;
use crate::colorprint::print_error;
use crate::models::{Disk, Move, Square};
use crate::settings::PlayerSettings;

/// Defines one player that can be either human or computer controlled.
#[derive(Debug)]
pub struct Player {
    pub can_play: bool,
    disk: Disk,
    player_type: PlayerType,
    rounds_played: usize,
    settings: PlayerSettings,
}

/// Player can be controlled either by a human or computer.
#[derive(Debug, PartialEq, Eq)]
pub enum PlayerType {
    Human,
    Computer,
}

impl Player {
    /// Initialize new player for the given disk color.
    pub const fn new(disk: Disk, settings: PlayerSettings, player_type: PlayerType) -> Self {
        Self {
            can_play: true,
            disk,
            player_type,
            rounds_played: 0,
            settings,
        }
    }

    /// Shorthand to initialize a new player for black disks.
    pub const fn black(settings: PlayerSettings) -> Self {
        Self::new(Disk::Black, settings, PlayerType::Human)
    }

    /// Shorthand to initialize a new player for white disks.
    pub const fn white(settings: PlayerSettings) -> Self {
        Self::new(Disk::White, settings, PlayerType::Computer)
    }

    /// Play one round as this player.
    pub fn play_one_move(&mut self, board: &mut Board) -> Option<String> {
        if !self.settings.check_mode {
            println!("Turn: {}", self.disk);
        }
        let moves = board.possible_moves(self.disk);
        if moves.is_empty() {
            self.can_play = false;
            if !self.settings.check_mode {
                println!("{}", "  No moves available...".yellow());
            }
            None
        } else {
            self.can_play = true;
            if self.human() && self.settings.show_helpers && !self.settings.check_mode {
                board.print_possible_moves(&moves);
            }
            let chosen_move = if self.human() {
                self.get_human_move(&moves)
            } else {
                self.get_computer_move(&moves)
            };
            board.place_disk(chosen_move);
            if !self.settings.check_mode {
                board.print_score();
            }
            self.rounds_played += 1;
            if !self.settings.test_mode {
                thread::sleep(Duration::from_secs(1));
            }
            Some(chosen_move.log_entry())
        }
    }

    /// Reset player status for a new game.
    pub const fn reset(&mut self) {
        self.can_play = true;
        self.rounds_played = 0;
    }

    /// Returns true if player is controlled by a human player.
    pub fn human(&self) -> bool {
        self.player_type.human()
    }

    /// Returns true if player is controlled by computer.
    #[allow(unused)]
    pub fn computer(&self) -> bool {
        self.player_type.computer()
    }

    /// Set the player as human or computer controlled.
    pub const fn set_player_type(&mut self, player_type: PlayerType) {
        self.player_type = player_type;
    }

    /// Set the player as human controlled.
    #[allow(unused)]
    pub const fn set_human(&mut self) {
        self.set_player_type(PlayerType::Human);
    }

    /// Set the player as computer controlled.
    pub const fn set_computer(&mut self) {
        self.set_player_type(PlayerType::Computer);
    }

    /// Return move chosen by computer.
    ///
    /// Lifetime annotation `<'a>` simply tells that the returned reference to a `Move`
    /// needs to live as long as the list of moves given as input.
    /// This way we avoid an unnecessary clone since the return value is not consumed by the caller.
    fn get_computer_move<'a>(&self, moves: &'a [Move]) -> &'a Move {
        if !self.settings.check_mode {
            println!("  Computer plays...");
        }
        let chosen_move = if self.settings.test_mode {
            &moves[0]
        } else {
            // Wait a bit and pick a random move
            thread::sleep(Duration::from_millis(rand::rng().random_range(1000..2000)));
            &moves[rand::rng().random_range(0..moves.len())]
        };
        if !self.settings.check_mode {
            println!("  {} -> {}", chosen_move.square, chosen_move.value);
        }
        chosen_move
    }

    /// Return move chosen by a human player.
    ///
    /// Lifetime annotation `<'a>` simply tells that the returned reference to a `Move`
    /// needs to live as long as the list of moves given as input.
    /// This way we avoid an unnecessary clone since the return value is not consumed by the caller.
    fn get_human_move<'a>(&self, moves: &'a [Move]) -> &'a Move {
        loop {
            let square = Self::get_square();
            // Check that the chosen square is actually one of the possible moves
            if let Some(valid_move) = moves.iter().find(|m| m.square == square) {
                return valid_move;
            }
            print_error(&format!(
                "  Can't place a {} disk in square {square}!",
                self.disk
            ));
        }
    }

    /// Ask human player for square coordinates.
    fn get_square() -> Square {
        loop {
            print!("  Give disk position (x,y): ");
            // Flush to ensure the message is displayed before reading input.
            io::stdout().flush().expect("Failed to flush stdout.");

            let mut input = String::new();
            if io::stdin().read_line(&mut input).is_err() {
                print_error("  Input failed. Please try again.");
                continue;
            }

            let values: Vec<&str> = input.trim().split(',').collect();
            if values.len() == 2 {
                let x: isize = values[0].parse().unwrap_or(-1);
                let y: isize = values[1].parse().unwrap_or(-1);
                if x >= 0 && y >= 0 {
                    return Square { x, y };
                }
            }
            print_error("  Give coordinates in the form 'x,y'!");
        }
    }

    /// Return player type description string.
    pub fn type_string(&self) -> String {
        self.player_type.to_string()
    }
}

impl Default for Player {
    fn default() -> Self {
        Self {
            can_play: true,
            disk: Disk::Black,
            player_type: PlayerType::Human,
            rounds_played: 0,
            settings: PlayerSettings::default(),
        }
    }
}

impl PlayerType {
    /// Check if the player is controlled by a human.
    pub fn human(&self) -> bool {
        *self == Self::Human
    }

    /// Check if the player is controlled by a computer.
    pub fn computer(&self) -> bool {
        *self == Self::Computer
    }
}

impl fmt::Display for Player {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(
            f,
            "{} | {} | Moves: {}",
            self.disk,
            self.type_string(),
            self.rounds_played
        )
    }
}

impl fmt::Display for PlayerType {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Human => write!(f, "Human   "),
            Self::Computer => write!(f, "Computer"),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn new_player() {
        let player = Player::default();
        assert_eq!(player.disk, Disk::Black);
        assert!(player.human());
        assert!(player.can_play);
        assert_eq!(player.rounds_played, 0);
        assert_eq!(player.settings, PlayerSettings::default());
    }

    #[test]
    fn reset_player() {
        let mut player = Player::new(Disk::White, PlayerSettings::default(), PlayerType::Human);
        player.can_play = false;
        player.rounds_played = 10;
        player.reset();
        assert!(player.can_play);
        assert_eq!(player.rounds_played, 0);
    }

    #[test]
    fn set_human() {
        let mut player = Player::default();
        player.set_computer();
        assert!(player.computer());
        assert_eq!(player.player_type, PlayerType::Computer);
        player.set_human();
        assert!(player.human());
        assert_eq!(player.player_type, PlayerType::Human);
    }

    #[test]
    fn player_type_string() {
        let mut player = Player::default();
        assert_eq!(player.type_string(), "Human   ");

        player.set_computer();
        assert_eq!(player.type_string(), "Computer");
    }
}

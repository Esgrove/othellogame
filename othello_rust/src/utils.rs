//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
// 2019-2023
//==========================================================

use colored::{Color, ColoredString, Colorize};
use sha2::{Digest, Sha256};
use std::cmp::Ordering;
use std::fmt;
use std::hash::{Hash, Hasher};
use std::ops::{Add, AddAssign};

use crate::player::PlayerSettings;

pub const MIN_BOARD_SIZE: usize = 4;
pub const MAX_BOARD_SIZE: usize = 10;
pub const DEFAULT_BOARD_SIZE: usize = 8;

/// Represents one game piece or lack of one.
#[derive(Eq, Debug, Copy, Clone, Hash, PartialEq)]
pub enum Disk {
    Black = -1,
    Empty = 0,
    White = 1,
}

/// Represents one step direction on the board.
#[derive(Eq, Debug, Copy, Clone, Hash, PartialEq, Ord, PartialOrd)]
pub struct Step {
    pub x: isize,
    pub y: isize,
}

/// Represents one square location on the board.
#[derive(Eq, Debug, Copy, Clone, Hash, PartialEq, Ord, PartialOrd)]
pub struct Square {
    pub x: isize,
    pub y: isize,
}

/// Represents one possible disk placement for the given disk color.
#[derive(Clone)]
pub struct Move {
    pub square: Square,
    pub disk: Disk,
    pub value: u32,
    pub directions: Vec<Step>,
}

/// Game settings
#[derive(Debug, Copy, Clone)]
pub struct Settings {
    pub board_size: usize,
    pub test_mode: bool,
    pub autoplay_mode: bool,
    pub quick_start: bool,
    pub show_helpers: bool,
    pub show_log: bool,
}

impl Settings {
    /// Get player setting values from overall game settings.
    pub fn to_player_settings(self) -> PlayerSettings {
        PlayerSettings {
            show_helpers: self.show_helpers,
            test_mode: self.test_mode,
        }
    }
}

impl Default for Settings {
    fn default() -> Self {
        Settings {
            board_size: 8,
            test_mode: false,
            autoplay_mode: false,
            quick_start: false,
            show_helpers: true,
            show_log: false,
        }
    }
}

impl Disk {
    /// Returns a single character identifier string for the given disk.
    pub fn board_char(&self) -> String {
        match self {
            Disk::Black => String::from("B"),
            Disk::Empty => String::from("_"),
            Disk::White => String::from("W"),
        }
    }

    /// Returns a single character identifier string for the given disk.
    pub fn board_char_with_color(&self) -> ColoredString {
        self.board_char().color(self.color())
    }

    /// Return the associated color for this disk.
    pub fn color(&self) -> Color {
        match self {
            Disk::Black => Color::Magenta,
            Disk::Empty => Color::White,
            Disk::White => Color::Cyan,
        }
    }

    /// Returns the disk formatted as a colored string.
    pub fn disk_string(&self) -> ColoredString {
        match self {
            Disk::Black => "BLACK".color(self.color()),
            Disk::Empty => "EMPTY".color(self.color()),
            Disk::White => "WHITE".color(self.color()),
        }
    }

    /// Return the opposing disk color for this disk.
    pub fn opponent(&self) -> Disk {
        match self {
            Disk::Black => Disk::White,
            Disk::Empty => Disk::Empty,
            Disk::White => Disk::Black,
        }
    }
}

impl Move {
    pub fn to_log_entry(&self) -> String {
        format!("{}:{},{}", self.disk.board_char(), self.square, self.value)
    }
}

impl fmt::Display for Disk {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.disk_string())
    }
}

impl fmt::Display for Square {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "({},{})", self.x, self.y)
    }
}

impl fmt::Display for Move {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Square: {} -> value: {}", self.square, self.value)
    }
}

impl Add for Square {
    type Output = Self;

    fn add(self, other: Self) -> Self {
        Self {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

impl Add<Step> for Square {
    type Output = Self;

    fn add(self, other: Step) -> Self {
        Self {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

impl Add<Step> for Step {
    type Output = Self;

    fn add(self, other: Step) -> Self {
        Self {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

impl AddAssign for Square {
    fn add_assign(&mut self, other: Self) {
        *self = Self {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

impl AddAssign<Step> for Square {
    fn add_assign(&mut self, other: Step) {
        *self = Self {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

impl AddAssign<Step> for Step {
    fn add_assign(&mut self, other: Self) {
        *self = Self {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

impl Eq for Move {}

impl Hash for Move {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.square.hash(state);
        self.value.hash(state);
        self.disk.hash(state);
        self.directions.hash(state);
    }
}

impl Ord for Move {
    fn cmp(&self, other: &Self) -> Ordering {
        other
            .value
            .cmp(&self.value)
            .then(self.square.cmp(&other.square))
    }
}

impl PartialEq for Move {
    fn eq(&self, other: &Self) -> bool {
        self.square == other.square && self.value == other.value
    }
}

impl PartialOrd for Move {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        // This uses `Ord` so no need to re-implement same comparison twice
        Some(self.cmp(other))
    }
}

/// Calculate SHA256 hash for the given string.
pub fn calculate_sha256(formatted_log: &String) -> String {
    let hash = Sha256::digest(formatted_log.as_bytes());
    base16ct::lower::encode_string(&hash)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn step_addition() {
        let mut result = Step { x: 0, y: 0 } + Step { x: 1, y: 1 };
        assert_eq!(result, Step { x: 1, y: 1 });

        result = Step { x: -1, y: 0 } + Step { x: 1, y: 0 };
        assert_eq!(result, Step { x: 0, y: 0 });

        result += Step { x: -1, y: -1 };
        assert_eq!(result, Step { x: -1, y: -1 });

        result += Step { x: 1, y: 1 };
        assert_eq!(result, Step { x: 0, y: 0 });
    }

    #[test]
    fn square_addition() {
        let mut result = Square { x: 4, y: 4 } + Square { x: 1, y: 1 };
        assert_eq!(result, Square { x: 5, y: 5 });

        result = Square { x: 4, y: 4 } + Square { x: 0, y: 0 };
        assert_eq!(result, Square { x: 4, y: 4 });

        result = Square { x: 4, y: 4 } + Step { x: -1, y: 1 };
        assert_eq!(result, Square { x: 3, y: 5 });

        result += Square { x: 0, y: 0 };
        assert_eq!(result, Square { x: 3, y: 5 });

        result += Step { x: -1, y: -1 };
        assert_eq!(result, Square { x: 2, y: 4 });

        result += Step { x: -1, y: -1 };
        assert_eq!(result, Square { x: 1, y: 3 });

        result += Step { x: -1, y: -1 };
        assert_eq!(result, Square { x: 0, y: 2 });

        result += Step { x: -1, y: -1 };
        assert_eq!(result, Square { x: -1, y: 1 });

        result += Step { x: 1, y: -1 };
        assert_eq!(result, Square { x: 0, y: 0 });

        result += Step { x: -1, y: -1 };
        assert_eq!(result, Square { x: -1, y: -1 });
    }
}

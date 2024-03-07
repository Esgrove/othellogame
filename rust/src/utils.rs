//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
// 2019-2024
//==========================================================

use crate::player::PlayerSettings;

use colored::{Color, ColoredString, Colorize};
use sha2::{Digest, Sha256};

use std::cmp::Ordering;
use std::fmt;
use std::hash::{Hash, Hasher};
use std::ops::{Add, AddAssign};

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
#[derive(Clone, Debug)]
pub struct Move {
    pub square: Square,
    pub disk: Disk,
    pub value: usize,
    pub directions: Vec<(Step, usize)>,
}

/// Game settings
#[derive(Debug, Copy, Clone)]
pub struct Settings {
    pub board_size: usize,
    pub check_mode: bool,
    pub test_mode: bool,
    pub autoplay_mode: bool,
    pub use_defaults: bool,
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
            check_mode: false,
            test_mode: false,
            autoplay_mode: false,
            use_defaults: false,
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
    /// Format move for log entry
    pub fn log_entry(&self) -> String {
        format!("{}:{},{}", self.disk.board_char(), self.square, self.value)
    }

    /// Get all the squares playing this move will change.
    pub fn affected_squares(&self) -> Vec<Square> {
        // Calculate the required size for the vector
        let total_size: usize = self.directions.iter().map(|(_, size)| size).sum();
        let mut paths: Vec<Square> = Vec::with_capacity(total_size);
        for &(step, count) in self.directions.iter() {
            let mut pos: Square = self.square + step;
            for _ in 0..count {
                paths.push(pos);
                pos += step;
            }
        }
        paths.sort();
        paths
    }
}

impl Square {
    #[allow(dead_code)]
    /// Get the index of this square on the board.
    pub fn board_index(&self, board_size: usize) -> usize {
        self.y as usize * board_size + self.x as usize
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
        self.square == other.square && self.value == other.value && self.disk == other.disk
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

        let another = Square { x: -3, y: -2 } + Square { x: 2, y: 3 };
        assert_eq!(another, Square { x: -1, y: 1 });

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

    #[test]
    fn test_step_directions() {
        let origin = Square { x: 1, y: 1 };
        let step_directions = [
            Step { x: -1, y: -1 },
            Step { x: -1, y: 0 },
            Step { x: -1, y: 1 },
            Step { x: 0, y: -1 },
            Step { x: 0, y: 1 },
            Step { x: 1, y: -1 },
            Step { x: 1, y: 0 },
            Step { x: 1, y: 1 },
        ];

        let expected_results = [
            Square { x: 0, y: 0 },
            Square { x: 0, y: 1 },
            Square { x: 0, y: 2 },
            Square { x: 1, y: 0 },
            Square { x: 1, y: 2 },
            Square { x: 2, y: 0 },
            Square { x: 2, y: 1 },
            Square { x: 2, y: 2 },
        ];

        for (i, step) in step_directions.iter().enumerate() {
            let result = origin + *step;
            assert_eq!(result, expected_results[i], "Step direction: {:?}", step);
        }
    }

    #[test]
    fn disk_board_char() {
        assert_eq!(Disk::Black.board_char(), "B");
        assert_eq!(Disk::Empty.board_char(), "_");
        assert_eq!(Disk::White.board_char(), "W");
    }

    #[test]
    fn disk_opponent() {
        assert_eq!(Disk::Black.opponent(), Disk::White);
        assert_eq!(Disk::Empty.opponent(), Disk::Empty);
        assert_eq!(Disk::White.opponent(), Disk::Black);
    }

    #[test]
    fn move_log_entry() {
        let b = Move {
            square: Square { x: 3, y: 2 },
            disk: Disk::Black,
            value: 10,
            directions: vec![(Step { x: 1, y: 0 }, 10)],
        };
        assert_eq!(b.log_entry(), "B:(3,2),10");

        let w = Move {
            square: Square { x: 0, y: 0 },
            disk: Disk::White,
            value: 1,
            directions: vec![(Step { x: 1, y: 0 }, 1)],
        };
        assert_eq!(w.log_entry(), "W:(0,0),1");
    }

    #[test]
    fn test_calculate_sha256() {
        let test_string = String::from("test");
        let hash = calculate_sha256(&test_string);
        assert_eq!(
            hash,
            "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08"
        );

        let test_string = String::from("01: B:(0,1),1;____BBB__BW_____");
        let hash = calculate_sha256(&test_string);
        assert_eq!(
            hash,
            "d087cd890b81b7153cc870fba7372396e91cdcd4b6d13b82bbc88b5a4eb889d1"
        );

        let mut test_log = String::new();
        test_log.push_str("01: B:(0,1),1;____BBB__BW_____\n");
        test_log.push_str("02: W:(0,0),1;W___BWB__BW_____\n");
        test_log.push_str("03: B:(1,0),1;WB__BBB__BW_____\n");
        test_log.push_str("04: W:(0,2),2;WB__WBB_WWW_____\n");
        test_log.push_str("05: B:(0,3),1;WB__WBB_WBW_B___\n");
        test_log.push_str("06: W:(2,0),3;WWW_WWW_WBW_B___\n");
        test_log.push_str("07: B:(3,0),1;WWWBWWB_WBW_B___\n");
        test_log.push_str("08: W:(1,3),1;WWWBWWB_WWW_BW__\n");
        test_log.push_str("09: B:(2,3),2;WWWBWWB_WWB_BBB_\n");
        test_log.push_str("10: W:(3,2),2;WWWBWWW_WWWWBBB_\n");
        test_log.push_str("11: B:(3,1),1;WWWBWWWBWWBWBBB_\n");
        test_log.push_str("12: W:(3,3),1;WWWBWWWBWWWWBBBW");
        let hash = calculate_sha256(&test_log);
        assert_eq!(
            hash,
            "ad4679949110ed7997aa1bf09441a7eb491b498189c03685ffdd6ddbb70e3c22"
        );
    }
}

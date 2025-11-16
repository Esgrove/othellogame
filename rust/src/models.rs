//! # Types
//! Basic types and methods.
//!
//! Akseli Lukkarila
//! 2019-2025

use std::cmp::Ordering;
use std::fmt;
use std::hash::{Hash, Hasher};
use std::ops::{Add, AddAssign};

use colored::{Color, ColoredString, Colorize};

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

/// Represents a continuous line of squares in one direction.
///
/// The `step` field determines the direction on the board,
/// and `count` describes how many consecutive squares in that direction there are.
#[derive(Eq, Debug, Copy, Clone, Hash, PartialEq, Ord, PartialOrd)]
pub struct Direction {
    /// Direction of travel on the board
    pub step: Step,
    /// Number of consecutive same colour squares along this direction
    pub count: usize,
}

/// Represents one possible disk placement for the given disk colour.
#[derive(Clone, Debug)]
pub struct Move {
    pub square: Square,
    pub disk: Disk,
    pub value: usize,
    pub directions: Vec<Direction>,
}

impl Disk {
    /// Returns a single character identifier string for the given disk.
    pub fn board_char(self) -> String {
        match self {
            Self::Black => String::from("B"),
            Self::Empty => String::from("_"),
            Self::White => String::from("W"),
        }
    }

    /// Returns a single character identifier string for the given disk.
    pub fn board_char_with_color(self) -> ColoredString {
        self.board_char().color(self.color())
    }

    /// Return the associated colour for this disk.
    pub const fn color(self) -> Color {
        match self {
            Self::Black => Color::Magenta,
            Self::Empty => Color::White,
            Self::White => Color::Cyan,
        }
    }

    /// Returns the disk formatted as a coloured string.
    pub fn disk_string(self) -> ColoredString {
        match self {
            Self::Black => "BLACK".color(self.color()),
            Self::Empty => "EMPTY".color(self.color()),
            Self::White => "WHITE".color(self.color()),
        }
    }

    /// Return the opposing disk colour for this disk.
    pub const fn opponent(self) -> Self {
        match self {
            Self::Black => Self::White,
            Self::Empty => Self::Empty,
            Self::White => Self::Black,
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
        let total_size: usize = self.directions.iter().map(|dir| dir.count).sum();
        let mut paths: Vec<Square> = Vec::with_capacity(total_size);
        for &Direction { step, count } in &self.directions {
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
    pub const fn board_index(&self, board_size: usize) -> usize {
        self.y as usize * board_size + self.x as usize
    }
}

impl Direction {
    /// Create a new `Direction` from a step and a count.
    pub const fn new(step: Step, count: usize) -> Self {
        Self { step, count }
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

impl Add<Self> for Step {
    type Output = Self;

    fn add(self, other: Self) -> Self {
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

impl AddAssign<Self> for Step {
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

#[cfg(test)]
mod tests {

    use crate::models::{Direction, Disk, Move, Square, Step};

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
    fn step_directions() {
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

        for (i, step) in step_directions.into_iter().enumerate() {
            let result = origin + step;
            assert_eq!(result, expected_results[i], "Step direction: {step:?}");
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
            directions: vec![Direction {
                step: Step { x: 1, y: 0 },
                count: 10,
            }],
        };
        assert_eq!(b.log_entry(), "B:(3,2),10");

        let w = Move {
            square: Square { x: 0, y: 0 },
            disk: Disk::White,
            value: 1,
            directions: vec![Direction {
                step: Step { x: 1, y: 0 },
                count: 1,
            }],
        };
        assert_eq!(w.log_entry(), "W:(0,0),1");
    }
}

//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
//==========================================================

use colored::{Color, ColoredString, Colorize};
use std::cmp::Ordering;
use std::fmt;
use std::hash::{Hash, Hasher};
use std::ops::{Add, AddAssign};

/// Represents one game piece or lack of one.
#[derive(Eq, Debug, Copy, Clone, Hash, PartialEq)]
pub(crate) enum Disk {
    Black = -1,
    Empty = 0,
    White = 1,
}

/// Represents one step direction on the board.
#[derive(Eq, Debug, Copy, Clone, Hash, PartialEq, Ord, PartialOrd)]
pub(crate) struct Step {
    pub(crate) x: isize,
    pub(crate) y: isize,
}

/// Represents one square location on the board.
#[derive(Eq, Debug, Copy, Clone, Hash, PartialEq, Ord, PartialOrd)]
pub(crate) struct Square {
    pub(crate) x: isize,
    pub(crate) y: isize,
}

/// Represents one possible disk placement for given disk color.
#[derive(Clone)]
pub(crate) struct Move {
    pub(crate) square: Square,
    pub(crate) disk: Disk,
    pub(crate) value: u32,
    pub(crate) directions: Vec<Step>,
}

impl Disk {
    /// Returns a single character identifier string for the given disk.
    pub(crate) fn board_char(&self) -> ColoredString {
        match self {
            Disk::Black => "B".color(self.color()),
            Disk::Empty => "_".color(self.color()),
            Disk::White => "W".color(self.color()),
        }
    }

    /// Return the associated color for this disk.
    pub(crate) fn color(&self) -> Color {
        match self {
            Disk::Black => Color::Magenta,
            Disk::Empty => Color::White,
            Disk::White => Color::Cyan,
        }
    }

    /// Returns the disk formatted as a colored string.
    pub(crate) fn disk_string(&self) -> ColoredString {
        match self {
            Disk::Black => "BLACK".color(self.color()),
            Disk::Empty => "EMPTY".color(self.color()),
            Disk::White => "WHITE".color(self.color()),
        }
    }

    /// Return the opposing disk color for this disk.
    pub(crate) fn opponent(&self) -> Disk {
        match self {
            Disk::Black => Disk::White,
            Disk::Empty => Disk::Empty,
            Disk::White => Disk::Black,
        }
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

impl PartialEq for Move {
    fn eq(&self, other: &Self) -> bool {
        self.square == other.square && self.value == other.value
    }
}

impl Eq for Move {}

impl PartialOrd for Move {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Option::from(
            self.value
                .cmp(&other.value)
                .then(self.square.partial_cmp(&other.square).unwrap()),
        )
    }
}

impl Ord for Move {
    fn cmp(&self, other: &Self) -> Ordering {
        self.value
            .cmp(&other.value)
            .then(self.square.cmp(&other.square))
    }
}

impl Hash for Move {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.square.hash(state);
        self.value.hash(state);
        self.disk.hash(state);
        self.directions.hash(state);
    }
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

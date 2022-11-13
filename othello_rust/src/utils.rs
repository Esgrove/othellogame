//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
//==========================================================

use colored::{Color, ColoredString, Colorize};
use std::cmp::Ordering;
use std::fmt;
use std::ops::{Add, AddAssign};

#[derive(Eq, Debug, Copy, Clone, Hash, PartialEq)]
pub(crate) enum Disk {
    BLACK = -1,
    EMPTY = 0,
    WHITE = 1,
}

#[derive(Eq, Debug, Copy, Clone, Hash, PartialEq, Ord, PartialOrd)]
pub(crate) struct Square {
    pub(crate) x: isize,
    pub(crate) y: isize,
}

#[derive(Eq, Debug, Copy, Clone, Hash, PartialEq, Ord, PartialOrd)]
pub(crate) struct Step {
    pub(crate) x: isize,
    pub(crate) y: isize,
}

#[derive(Clone, Hash)]
pub(crate) struct Move {
    pub(crate) square: Square,
    pub(crate) value: u32,
    pub(crate) disk: Disk,
    pub(crate) directions: Vec<Step>,
}

impl Disk {
    /// Returns a single character identifier string for the given disk.
    pub(crate) fn board_char(&self) -> ColoredString {
        match self {
            Disk::BLACK => "B".magenta(),
            Disk::EMPTY => "_".white(),
            Disk::WHITE => "W".cyan(),
        }
    }

    /// Return the associated color for this disk.
    pub(crate) fn color(&self) -> Color {
        match self {
            Disk::BLACK => Color::Magenta,
            Disk::EMPTY => Color::White,
            Disk::WHITE => Color::Cyan,
        }
    }

    /// Returns the disk formatted as a colored string.
    pub(crate) fn disk_string(&self) -> ColoredString {
        match self {
            Disk::BLACK => "BLACK".magenta(),
            Disk::EMPTY => "EMPTY".white(),
            Disk::WHITE => "WHITE".cyan(),
        }
    }

    /// Return the opposing disk color for this disk.
    pub(crate) fn opponent(&self) -> Disk {
        match self {
            Disk::BLACK => Disk::WHITE,
            Disk::EMPTY => Disk::EMPTY,
            Disk::WHITE => Disk::BLACK,
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
        write!(f, "({}, {})", self.x, self.y)
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

impl PartialEq for Move {
    fn eq(&self, other: &Self) -> bool {
        self.square == other.square && self.value == other.value
    }

    fn ne(&self, other: &Self) -> bool {
        self.square != other.square || self.value != other.value
    }
}

impl Eq for Move {}

impl PartialOrd for Move {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Option::from(
            self.square
                .partial_cmp(&other.square)
                .unwrap()
                .then(self.value.cmp(&other.value)),
        )
    }
}

impl Ord for Move {
    fn cmp(&self, other: &Self) -> Ordering {
        self.square.cmp(&other.square).then(self.value.cmp(&other.value))
    }
}

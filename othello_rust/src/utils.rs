//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
//==========================================================

use colored::{Color, ColoredString, Colorize};
use std::fmt;

#[derive(Debug, Copy, Clone, PartialEq)]
pub(crate) enum Disk {
    BLACK = -1,
    EMPTY = 0,
    WHITE = 1,
}

#[derive(Debug, Copy, Clone, Eq, Hash, PartialEq)]
pub(crate) struct Square {
    pub(crate) x: usize,
    pub(crate) y: usize,
}

#[derive(Eq, Copy, Clone, Hash, PartialEq)]
pub(crate) struct Move {
    pub(crate) square: Square,
    pub(crate) value: u32,
}

impl Disk {
    /// Returns a single character identifier string for the given disk.
    fn board_char(&self) -> ColoredString {
        match self {
            Disk::BLACK => "B".magenta(),
            Disk::EMPTY => "_".normal(),
            Disk::WHITE => "W".cyan(),
        }
    }

    /// Return the associated color for this disk.
    fn color(&self) -> Color {
        match self {
            Disk::BLACK => Color::Magenta,
            Disk::EMPTY => Color::White,
            Disk::WHITE => Color::Cyan,
        }
    }

    /// Returns the disk formatted as a colored string.
    fn disk_string(&self) -> ColoredString {
        match self {
            Disk::BLACK => "BLACK".magenta(),
            Disk::EMPTY => "EMPTY".normal(),
            Disk::WHITE => "NORMAL".cyan(),
        }
    }

    /// Return the opposing disk color for this disk.
    fn opponent(&self) -> Disk {
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

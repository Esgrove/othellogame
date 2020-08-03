//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
//==========================================================

use std::fmt;

#[derive(Debug, Copy, Clone)]
pub(crate) enum Disk {
    BLACK = -1,
    EMPTY = 0,
    WHITE = 1
}

pub(crate) struct Square {
    x: u8,
    y: u8
}

pub(crate) struct Move {
    square: Square,
    value: u8
}

impl fmt::Display for Disk {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{:?}", self)
    }
}

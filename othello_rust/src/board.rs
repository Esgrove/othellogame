//==========================================================
// Board
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

extern crate colored; // print with color to terminal

use std::fmt;

use crate::utils::Disk;

pub(crate) struct Board {
    board: Vec<Disk>,
    size: u8,
}

impl Board {
    pub(crate) fn default() -> Board {
        Board {
            board: vec![Disk::EMPTY; 64usize],
            size: 8,
        }
    }

    pub(crate) fn new(size: u8) -> Board {
        Board {
            board: vec![Disk::EMPTY; (size * size) as usize],
            size,
        }
    }
}

impl fmt::Display for Board {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Board: {}", self.size)
    }
}

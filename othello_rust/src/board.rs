//==========================================================
// Board
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

extern crate colored; // print with color to terminal

use crate::utils::{Disk};

pub struct Board {
    board: Vec<Disk>,
    size: u8
}

impl Board {
    pub fn default() -> Board {
        Board {
            board: vec![Disk::EMPTY; 64usize],
            size
        }
    }

    pub fn new(size: u8) -> Board {
        Board {
            board: vec![Disk::EMPTY; (size * size) as usize],
            size
        }
    }
}

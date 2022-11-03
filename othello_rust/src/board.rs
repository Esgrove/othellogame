//==========================================================
// Board
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

extern crate colored;

use std::collections::HashSet;
use std::fmt;

use crate::utils::{Disk, Move, Square};

pub(crate) struct Board {
    board: Vec<Disk>,
    size: u8,
    empty_squares: HashSet<Square>,
    indices: Vec<u8>,
}

impl Board {
    pub(crate) fn default() -> Board {
        Board {
            board: vec![Disk::EMPTY; 64usize],
            size: 8,
            empty_squares: HashSet::new(),
            indices: Vec::from_iter(0..8),
        }
    }

    pub(crate) fn new(size: u8) -> Board {
        Board {
            board: vec![Disk::EMPTY; (size * size) as usize],
            size,
            empty_squares: HashSet::new(),
            indices: Vec::from_iter(0..size),
        }
    }

    pub(crate) fn can_play(&mut self) -> bool {
        !self.empty_squares.is_empty()
    }

    // TODO
    pub(crate) fn possible_moves(disk: Disk) -> Vec<Move> {
        let mut moves = Vec::<Move>::new();
        return moves;
    }

    /// Returns the winner color.
    pub(crate) fn result(&mut self) -> Disk {
        let sum = self.score();
        if sum == 0 {
            return Disk::EMPTY;
        }
        if sum > 0 { Disk::WHITE } else { Disk::BLACK }
    }

    // TODO
    /// Returns the total score (positive means more white disks and negative means more black disks).
    fn score(&mut self) -> u8 {
        0
    }
}

impl fmt::Display for Board {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Board: {}", self.size)
    }
}

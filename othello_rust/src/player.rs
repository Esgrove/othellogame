//==========================================================
// Player
// Play Othello (Reversi) on the command line
// Akseli Lukkarila
//==========================================================

extern crate colored;

use std::fmt;

use crate::board::Board;
use crate::utils::Disk;

#[derive(Debug)]
pub(crate) struct Player {
    color: Disk,
    human: bool,
    can_play: bool,
    show_helpers: bool,
    rounds_played: u32,
}

impl Player {
    pub fn new(color: Disk) -> Player {
        Player {
            color,
            human: true,
            can_play: true,
            show_helpers: true,
            rounds_played: 0,
        }
    }
    pub fn play_one_move(&mut self, board: &mut Board) {}

    pub fn set_human(&mut self, is_human: bool) {
        self.human = is_human;
    }

    pub fn can_play(&self) -> bool {
        self.can_play
    }

    pub fn type_string(&self) -> String {
        match self.human {
            true => "human   ",
            false => "computer",
        }
        .parse()
        .unwrap()
    }
}

impl fmt::Display for Player {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.color)
    }
}

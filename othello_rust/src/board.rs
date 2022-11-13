//==========================================================
// Board
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

extern crate colored;

use colored::{ColoredString, Colorize};
use std::collections::HashSet;
use std::fmt;

use crate::utils::{Disk, Move, Square, Step};

pub(crate) struct Board {
    board: Vec<Disk>,
    size: usize,
    empty_squares: HashSet<Square>,
    indices: Vec<usize>,
    step_directions: Vec<Step>,
}

impl Board {
    pub(crate) fn new(size: usize) -> Board {
        let board = Self::init_board(size);
        // index list (0...size) to avoid repeating same range in for loops
        let indices = Vec::from_iter(0..size);
        // keep track of empty squares on board to avoid checking already filled positions
        let mut empty_squares = HashSet::<Square>::new();
        for y in 0..size {
            for x in 0..size {
                if board[y * size + x] == Disk::EMPTY {
                    empty_squares.insert(Square {
                        x: x as isize,
                        y: y as isize,
                    });
                }
            }
        }
        Board {
            board,
            size,
            empty_squares,
            indices,
            step_directions: vec![
                Step { x: -1, y: -1 },
                Step { x: -1, y: 0 },
                Step { x: -1, y: 1 },
                Step { x: 0, y: -1 },
                Step { x: 0, y: 1 },
                Step { x: 1, y: -1 },
                Step { x: 1, y: 0 },
                Step { x: 1, y: 1 },
            ],
        }
    }

    /// Return true if board contains empty squares.
    pub(crate) fn can_play(&self) -> bool {
        !self.empty_squares.is_empty()
    }

    /// Check that the given coordinates are valid (inside the board).
    fn check_coordinates(&self, x: isize, y: isize) -> bool {
        x >= 0 && x <= self.size as isize && y >= 0 && y <= self.size as isize
    }

    /// Update board for given disk placement.
    pub(crate) fn place_disk(&mut self, player_move: &Move) {
        let start = player_move.square;
        if self
            .get_square(&start)
            .expect(&*format!("Invalid coordinates: {}", start))
            != Disk::EMPTY
        {
            panic!("Trying to place disk to an occupied square {}!", start);
        }
        self.set_square(&start, &player_move.disk);
        for step in &player_move.directions {
            let mut pos = start + *step;
            while self.get_square(&pos).unwrap_or(Disk::EMPTY) == player_move.disk.opponent() {
                self.set_square(&pos, &player_move.disk);
                pos += *step;
            }
        }
    }

    /// Returns a list of possible moves for given player.
    pub(crate) fn possible_moves(&self, disk: Disk) -> Vec<Move> {
        let mut moves = Vec::<Move>::new();
        let opposing_disk = disk.opponent();
        for square in &self.empty_squares {
            let mut value: u32 = 0;
            let mut directions = Vec::<Step>::new();
            for step in self.step_directions.iter() {
                let mut pos = *square + *step;
                // next square in this direction needs to be opponents disk
                if self.get_square(&pos).unwrap_or(Disk::EMPTY) != opposing_disk {
                    continue;
                }
                let mut num_steps: u32 = 0;
                while self.get_square(&pos).unwrap_or(Disk::EMPTY) == opposing_disk {
                    num_steps += 1;
                    pos += *step;
                }
                // valid move only if a line of opponents disks ends in own disk
                if self.get_square(&pos).unwrap_or(Disk::EMPTY) == disk {
                    value += num_steps;
                    directions.push(*step);
                }
            }
            if value > 0 {
                moves.push(Move {
                    square: *square,
                    value,
                    disk,
                    directions,
                });
            }
        }
        if !moves.is_empty() {
            moves.sort();
        }
        return moves;
    }

    /// Print board with available move coordinates and the resulting points gained.
    pub(crate) fn print_moves(&self, moves: &Vec<Move>) {
        println!("{}", format!("  Possible plays ({}):\n", moves.len()).yellow());
        // Convert board from Disk enums to strings
        let mut formatted_board: Vec<ColoredString> = self.board.iter().map(|&d| d.board_char()).collect();
        // Add possible moves to board
        for possible_move in moves {
            let index = possible_move.square.y as usize * self.size + possible_move.square.x as usize;
            formatted_board[index] = possible_move.value.to_string().yellow();
        }
        // Print board with move positions
        print!("   ");
        for i in 0..self.size {
            print!(" {}", i.to_string().bold());
        }
        for y in 0..self.size {
            print!("\n  {}", y.to_string().bold());
            for x in 0..self.size {
                print!(" {}", formatted_board[y * self.size + x]);
            }
        }
    }

    /// Print current score for both players.
    pub(crate) fn print_score(&self) {
        let (black, white) = self.player_scores();
        println!("\n{}", self);
        println!(
            "Score: {} | {}\n",
            black.to_string().magenta(),
            white.to_string().cyan()
        )
    }

    /// Count and return the number of black and white disks.
    fn player_scores(&self) -> (u32, u32) {
        let mut black: u32 = 0;
        let mut white: u32 = 0;
        for disk in self.board.iter() {
            match disk {
                Disk::BLACK => black += 1,
                Disk::WHITE => white += 1,
                _ => {}
            }
        }
        (black, white)
    }

    /// Returns the winner color.
    pub(crate) fn result(&self) -> Disk {
        let sum = self.score();
        if sum > 0 {
            Disk::WHITE
        } else if sum < 0 {
            Disk::BLACK
        } else {
            Disk::EMPTY
        }
    }

    /// Returns the total score (positive means more white disks and negative means more black disks).
    fn score(&self) -> i32 {
        self.board.iter().map(|d| *d as i32).sum()
    }

    /// Returns the state of the board (empty, white, black) at the given coordinates.
    fn get_square(&self, square: &Square) -> Option<Disk> {
        if self.check_coordinates(square.x, square.y) {
            let disk = self.board[square.y as usize * self.size + square.x as usize];
            return Some(disk);
        }
        None
    }

    /// Sets the given square to given value.
    fn set_square(&mut self, square: &Square, disk: &Disk) {
        if self.check_coordinates(square.x, square.y) {
            self.board[square.y as usize * self.size + square.x as usize] = *disk;
        }
        panic!("Invalid coordinates");
    }

    fn init_board(size: usize) -> Vec<Disk> {
        // init game board with empty disks.
        let mut board = vec![Disk::EMPTY; (size * size) as usize];
        // set starting positions
        let row = if size % 2 == 0 {
            (size - 1) / 2
        } else {
            (size - 1) / 2 - 1
        };
        let col = size / 2;
        board[row * size + row] = Disk::BLACK;
        board[row * size + col] = Disk::WHITE;
        board[col * size + row] = Disk::WHITE;
        board[col * size + col] = Disk::BLACK;
        board
    }
}

impl fmt::Display for Board {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        // Horizontal header indices
        let column_indices: Vec<String> = self.indices.iter().map(|&i| i.to_string()).collect();
        let mut text: String = format!("  {}", column_indices.join(" ").bold());
        for y in 0..self.size {
            // Vertical header index
            text += &*format!("\n{}", y.to_string().bold());
            // Output row values
            let row = &self.board[(y * self.size)..(y * self.size + self.size)];
            for disk in row {
                text += &*format!(" {}", disk.board_char());
            }
        }
        write!(f, "{}", text)
    }
}

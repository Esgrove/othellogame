//==========================================================
// Board
// Defines the game board
// Akseli Lukkarila
// 2019-2023
//==========================================================

extern crate colored;

use colored::{ColoredString, Colorize};
use std::cmp::Ordering;
use std::collections::HashSet;
use std::fmt;

use crate::utils::{Disk, Move, Square, Step};

/// Handles game board state and logic.
pub struct Board {
    board: Vec<Disk>,
    size: usize,
    empty_squares: HashSet<Square>,
    indices: Vec<usize>,
    step_directions: [Step; 8],
}

impl Board {
    pub fn new(size: usize) -> Board {
        let board = Self::init_board(size);
        // Index list (0...size) to avoid repeating same range in loops.
        // Not really needed in Rust but kept here to more closely match other implementations...
        let indices = Vec::from_iter(0..size);
        // Keep track of empty squares on board to avoid checking already filled positions.
        let mut empty_squares = HashSet::<Square>::new();
        for y in indices.iter() {
            for x in indices.iter() {
                if board[y * size + x] == Disk::Empty {
                    empty_squares.insert(Square {
                        x: *x as isize,
                        y: *y as isize,
                    });
                }
            }
        }
        Board {
            board,
            size,
            empty_squares,
            indices,
            step_directions: [
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
    pub fn can_play(&self) -> bool {
        !self.empty_squares.is_empty()
    }

    /// Update board for given disk placement.
    pub fn place_disk(&mut self, player_move: &Move) {
        let start = player_move.square;
        if self
            .get_square(&start)
            .unwrap_or_else(|| panic!("Invalid coordinates: {}", start))
            != Disk::Empty
        {
            panic!("Trying to place disk to an occupied square: {}!", start);
        }
        self.set_square(&start, &player_move.disk);
        self.empty_squares.remove(&start);
        for step in &player_move.directions {
            let mut pos = start + *step;
            while self.get_square(&pos).unwrap_or(Disk::Empty) == player_move.disk.opponent() {
                self.set_square(&pos, &player_move.disk);
                pos += *step;
            }
        }
    }

    /// Returns a list of possible moves for the given player.
    pub fn possible_moves(&self, disk: Disk) -> Vec<Move> {
        let mut moves = Vec::<Move>::new();
        let opposing_disk = disk.opponent();
        for square in &self.empty_squares {
            let mut value: u32 = 0;
            let mut directions = Vec::<Step>::new();
            for step in self.step_directions.iter() {
                let mut pos = *square + *step;
                // Next square in this direction needs to be the opposing disk
                if self.get_square(&pos).unwrap_or(Disk::Empty) != opposing_disk {
                    continue;
                }
                let mut num_steps: u32 = 0;
                // Keep stepping over opponents disks
                while self.get_square(&pos).unwrap_or(Disk::Empty) == opposing_disk {
                    num_steps += 1;
                    pos += *step;
                }
                // Valid move only if a line of opposing disks ends in own disk
                if self.get_square(&pos).unwrap_or(Disk::Empty) == disk {
                    value += num_steps;
                    directions.push(*step);
                }
            }
            if value > 0 {
                moves.push(Move {
                    square: *square,
                    disk,
                    value,
                    directions,
                });
            }
        }
        if !moves.is_empty() {
            moves.sort();
        }
        moves
    }

    /// Print board with available move coordinates and the resulting points gained.
    pub fn print_possible_moves(&self, moves: &Vec<Move>) {
        println!(
            "{}",
            format!("  Possible moves ({}):", moves.len()).yellow()
        );
        // Convert board from Disk enums to strings
        let mut formatted_board: Vec<ColoredString> = self
            .board
            .iter()
            .map(|&d| d.board_char_with_color())
            .collect();
        // Add possible moves to board
        for possible_move in moves {
            let index =
                possible_move.square.y as usize * self.size + possible_move.square.x as usize;
            formatted_board[index] = possible_move.value.to_string().yellow();
            println!("  {}", possible_move);
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
        println!();
    }

    /// Print current score for both players.
    pub fn print_score(&self) {
        let (black, white) = self.player_scores();
        println!("\n{}", self);
        println!(
            "Score: {} | {}",
            black.to_string().magenta(),
            white.to_string().cyan()
        )
    }

    /// Returns the winning disk colour. Empty indicates a draw.
    pub fn result(&self) -> Disk {
        let sum = self.score();
        match sum.cmp(&0) {
            Ordering::Greater => Disk::White,
            Ordering::Less => Disk::Black,
            Ordering::Equal => Disk::Empty,
        }
    }

    /// Get board status string for game log.
    pub fn to_log_entry(&self) -> String {
        self.board.iter().map(|&d| d.board_char()).collect()
    }

    /// Check that the given coordinates are valid (inside the board).
    fn check_coordinates(&self, x: isize, y: isize) -> bool {
        x >= 0 && x < self.size as isize && y >= 0 && y < self.size as isize
    }

    /// Returns the state of the board (empty, white, black) at the given square.
    fn get_square(&self, square: &Square) -> Option<Disk> {
        if self.check_coordinates(square.x, square.y) {
            let disk = self.board[square.y as usize * self.size + square.x as usize];
            Some(disk)
        } else {
            None
        }
    }

    /// Count and return the number of black and white disks.
    fn player_scores(&self) -> (u32, u32) {
        let mut black: u32 = 0;
        let mut white: u32 = 0;
        for disk in self.board.iter() {
            match disk {
                Disk::Black => black += 1,
                Disk::White => white += 1,
                _ => {}
            }
        }
        (black, white)
    }

    /// Returns the total score.
    /// Positive value means more white disks and negative means more black disks.
    fn score(&self) -> i32 {
        self.board.iter().map(|d| *d as i32).sum()
    }

    /// Sets the given square to the given value.
    fn set_square(&mut self, square: &Square, disk: &Disk) {
        if !self.check_coordinates(square.x, square.y) {
            panic!("Invalid coordinates: {}", square);
        }
        self.board[square.y as usize * self.size + square.x as usize] = *disk;
    }

    /// Initialize game board with starting disk positions.
    fn init_board(size: usize) -> Vec<Disk> {
        // Initialize game board with empty disks
        let mut board = vec![Disk::Empty; size * size];
        // Set starting positions
        let row = if size % 2 == 0 {
            (size - 1) / 2
        } else {
            (size - 1) / 2 - 1
        };
        let col = size / 2;
        board[row * size + row] = Disk::White;
        board[row * size + col] = Disk::Black;
        board[col * size + row] = Disk::Black;
        board[col * size + col] = Disk::White;
        board
    }
}

impl fmt::Display for Board {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        // Horizontal indices
        let column_indices: Vec<String> = self.indices.iter().map(|&i| i.to_string()).collect();
        let mut text: String = format!("  {}", column_indices.join(" ").bold());
        for y in 0..self.size {
            // Vertical index
            text += &*format!("\n{}", y.to_string().bold());
            // Row values
            let row = &self.board[(y * self.size)..(y * self.size + self.size)];
            for disk in row {
                text += &*format!(" {}", disk.board_char_with_color());
            }
        }
        write!(f, "{}", text)
    }
}

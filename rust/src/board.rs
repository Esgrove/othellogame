//==========================================================
// Board
// Defines the game board
// Akseli Lukkarila
// 2019-2024
//==========================================================

use std::cmp::Ordering;
use std::collections::HashSet;
use std::fmt;

use colored::{ColoredString, Colorize};

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
    pub fn new(size: usize) -> Self {
        let board = Self::init_board(size);
        // Index list (0...size) to avoid repeating same range in loops.
        // Not really needed in Rust but kept here to more closely match other implementations...
        let indices: Vec<usize> = (0..size).collect();

        // Keep track of empty squares on board to avoid checking already filled positions.
        let empty_squares: HashSet<Square> = (0..size * size)
            .filter_map(|i| {
                if board[i] == Disk::Empty {
                    Some(Square {
                        x: (i % size) as isize,
                        y: (i / size) as isize,
                    })
                } else {
                    None
                }
            })
            .collect();

        Self {
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
            .unwrap_or_else(|| panic!("Invalid coordinates: {start}"))
            != Disk::Empty
        {
            panic!("Trying to place disk to an occupied square: {start}!");
        }
        self.set_square(&start, player_move.disk);
        self.empty_squares.remove(&start);
        for square in &player_move.affected_squares() {
            self.set_square(square, player_move.disk);
        }
    }

    /// Returns a list of possible moves for the given player.
    pub fn possible_moves(&self, disk: Disk) -> Vec<Move> {
        let mut moves: Vec<Move> = Vec::new();
        let opposing_disk = disk.opponent();
        for square in &self.empty_squares {
            let mut value: usize = 0;
            let mut directions: Vec<(Step, usize)> = Vec::new();
            for step in &self.step_directions {
                let mut pos = *square + *step;
                // Next square in this direction needs to be the opposing disk
                if self.get_square(&pos).unwrap_or(Disk::Empty) != opposing_disk {
                    continue;
                }
                let mut num_steps: usize = 0;
                // Keep stepping over opponents disks
                while self.get_square(&pos).unwrap_or(Disk::Empty) == opposing_disk {
                    num_steps += 1;
                    pos += *step;
                }
                // Valid move only if a line of opposing disks ends in own disk
                if self.get_square(&pos).unwrap_or(Disk::Empty) == disk {
                    directions.push((*step, num_steps));
                    value += num_steps;
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
            let index = self.square_index(&possible_move.square);
            formatted_board[index] = possible_move.value.to_string().yellow();
            println!("  {possible_move}");
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
        println!("\n{self}");
        println!(
            "Score: {} | {}",
            black.to_string().magenta(),
            white.to_string().cyan()
        );
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
    pub fn log_entry(&self) -> String {
        self.board.iter().map(|&d| d.board_char()).collect()
    }

    #[allow(dead_code)]
    /// Check that the given coordinates are valid (inside the board).
    const fn check_coordinates(&self, x: isize, y: isize) -> bool {
        x >= 0 && x < self.size as isize && y >= 0 && y < self.size as isize
    }

    /// Check that the given square is valid (inside the board).
    const fn check_square(&self, square: &Square) -> bool {
        square.x >= 0
            && square.x < self.size as isize
            && square.y >= 0
            && square.y < self.size as isize
    }

    /// Returns the state of the board (empty, white, black) at the given square.
    fn get_square(&self, square: &Square) -> Option<Disk> {
        if self.check_square(square) {
            let index = self.square_index(square);
            Some(self.board[index])
        } else {
            None
        }
    }

    #[allow(dead_code)]
    /// Map square to index on board.
    const fn square_index(&self, square: &Square) -> usize {
        square.y as usize * self.size + square.x as usize
    }

    /// Count and return the number of black and white disks.
    fn player_scores(&self) -> (usize, usize) {
        let mut black: usize = 0;
        let mut white: usize = 0;
        for disk in &self.board {
            match disk {
                Disk::Black => black += 1,
                Disk::White => white += 1,
                Disk::Empty => {}
            }
        }
        (black, white)
    }

    /// Returns the total score.
    /// Positive value means more white disks and negative means more black disks.
    fn score(&self) -> i32 {
        self.board.iter().map(|disk| *disk as i32).sum()
    }

    /// Sets the given square to the given value.
    fn set_square(&mut self, square: &Square, disk: Disk) {
        assert!(self.check_square(square), "Invalid coordinates: {square}");
        let index = self.square_index(square);
        self.board[index] = disk;
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
        // Could just use `0..self.size` here
        for y in &self.indices {
            // Vertical index
            text += &format!("\n{}", y.to_string().bold());
            // Row values
            let row = &self.board[(y * self.size)..(y * self.size + self.size)];
            for disk in row {
                text += &format!(" {}", disk.board_char_with_color());
            }
        }
        write!(f, "{text}")
    }
}

#[cfg(test)]
#[allow(clippy::unwrap_used)]
mod tests {
    use super::*;

    #[test]
    fn board_initialization() {
        let board_size = 8;
        let board = Board::new(board_size);
        assert_eq!(board.size, board_size);
        // Check initial disk positions
        assert_eq!(
            board.get_square(&Square { x: 3, y: 3 }).unwrap(),
            Disk::White
        );
        assert_eq!(
            board.get_square(&Square { x: 4, y: 4 }).unwrap(),
            Disk::White
        );
        assert_eq!(
            board.get_square(&Square { x: 3, y: 4 }).unwrap(),
            Disk::Black
        );
        assert_eq!(
            board.get_square(&Square { x: 4, y: 3 }).unwrap(),
            Disk::Black
        );
        // Rest of the board should be empty
        assert_eq!(
            board.get_square(&Square { x: 0, y: 0 }).unwrap(),
            Disk::Empty
        );
    }

    #[test]
    fn can_play() {
        let board = Board::new(8);
        assert!(board.can_play());
        // TODO: Fill the board and test again
    }

    #[test]
    fn scoring() {
        let board = Board::new(8);
        let (black_score, white_score) = board.player_scores();
        assert_eq!(black_score, 2);
        assert_eq!(white_score, 2);
        assert_eq!(board.score(), 0);
    }

    #[test]
    fn game_result() {
        let mut board = Board::new(8);
        let mut result = board.result();
        assert_eq!(result, Disk::Empty);

        let moves = board.possible_moves(Disk::Black);
        board.place_disk(&moves[0]);

        result = board.result();
        assert_eq!(result, Disk::Black);
    }

    #[test]
    fn log_entry() {
        let board = Board::new(8);
        let log_entry = board.log_entry();
        assert_eq!(
            log_entry,
            "___________________________WB______BW___________________________"
        );

        let mut board = Board::new(4);
        let log_entry = board.log_entry();
        assert_eq!(log_entry, "_____WB__BW_____");

        let moves = board.possible_moves(Disk::Black);
        board.place_disk(&moves[0]);
        let log_entry = board.log_entry();
        assert_eq!(log_entry, "____BBB__BW_____");
    }
}

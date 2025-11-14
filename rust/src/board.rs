//! # Board
//! Defines the game board.
//!
//! Akseli Lukkarila
//! 2019-2025

use std::cmp::Ordering;
use std::collections::HashSet;
use std::fmt;

use colored::{ColoredString, Colorize};

use crate::models::{Direction, Disk, Move, Square, Step};

const UP: isize = 1;
const DOWN: isize = -1;
const LEFT: isize = -1;
const RIGHT: isize = 1;
const STILL: isize = 0;

/// All possible step directions for a square on the board.
static STEP_DIRECTIONS: [Step; 8] = [
    Step { x: UP, y: LEFT },
    Step { x: UP, y: STILL },
    Step { x: UP, y: RIGHT },
    Step { x: STILL, y: LEFT },
    Step { x: STILL, y: RIGHT },
    Step { x: DOWN, y: LEFT },
    Step { x: DOWN, y: STILL },
    Step { x: DOWN, y: RIGHT },
];

/// Handles game board state and logic.
pub struct Board {
    board: Vec<Disk>,
    empty_squares: HashSet<Square>,
    indices: Box<[usize]>,
    size: usize,
}

impl Board {
    pub fn new(size: usize) -> Self {
        let board = Self::initialize_board(size);
        // Index list (0...size) to avoid repeating same range in loops.
        // Not really needed in Rust but kept here to more closely match other implementations...
        // Could just use `0..self.size` directly.
        let indices = (0..size).collect::<Vec<_>>().into_boxed_slice();

        // Keep track of empty squares on board to avoid checking already filled positions.
        let empty_squares = Self::initialize_empty_squares(size, &board);

        Self {
            board,
            empty_squares,
            indices,
            size,
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
            let mut directions: Vec<Direction> = Vec::new();
            for step in &STEP_DIRECTIONS {
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
                    directions.push(Direction::new(*step, num_steps));
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
        let header: String = std::iter::once("    ".to_string())
            .chain(
                self.indices
                    .iter()
                    .map(|i| format!("{}", i.to_string().bold())),
            )
            .collect::<Vec<_>>()
            .join(" ");

        let board: String = self
            .indices
            .iter()
            .map(|y| {
                let row = self
                    .indices
                    .iter()
                    .map(|x| formatted_board[y * self.size + x].to_string())
                    .collect::<Vec<_>>()
                    .join(" ");
                format!("  {} {}", y.to_string().bold(), row)
            })
            .collect::<Vec<_>>()
            .join("\n");

        println!("{header}\n{board}");
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
        let total_score = self.score();
        match total_score.cmp(&0) {
            Ordering::Greater => Disk::White,
            Ordering::Less => Disk::Black,
            Ordering::Equal => Disk::Empty,
        }
    }

    /// Get board status string for game log.
    pub fn log_entry(&self) -> String {
        self.board.iter().map(|&disk| disk.board_char()).collect()
    }

    /// Check that the given coordinates are valid (inside the board).
    const fn check_coordinates(&self, x: isize, y: isize) -> bool {
        x >= 0 && x < self.size as isize && y >= 0 && y < self.size as isize
    }

    /// Check that the given square is valid (inside the board).
    const fn check_square(&self, square: &Square) -> bool {
        self.check_coordinates(square.x, square.y)
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

    /// Map square to board index.
    const fn square_index(&self, square: &Square) -> usize {
        square.y as usize * self.size + square.x as usize
    }

    /// Count and return the number of black and white disks.
    fn player_scores(&self) -> (usize, usize) {
        // Alternative that is more readable but iterates the board twice:
        // let black = self.board.iter().filter(|&&disk| disk == Disk::Black).count();
        // let white = self.board.iter().filter(|&&disk| disk == Disk::White).count();
        let mut black = 0;
        let mut white = 0;
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
        self.board
            .iter()
            .filter(|&&disk| disk != Disk::Empty)
            .map(|disk| *disk as i32)
            .sum()
    }

    /// Sets the given square to the given value.
    fn set_square(&mut self, square: &Square, disk: Disk) {
        assert!(self.check_square(square), "Invalid coordinates: {square}");
        let index = self.square_index(square);
        self.board[index] = disk;
    }

    /// Initialize game board with starting disk positions.
    fn initialize_board(size: usize) -> Vec<Disk> {
        // Initialize game board with empty disks
        let mut board = vec![Disk::Empty; size * size];
        // Set starting positions
        let row = if size.is_multiple_of(2) {
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

    /// Initialize a set of empty squares for the board.
    fn initialize_empty_squares(size: usize, board: &[Disk]) -> HashSet<Square> {
        (0..board.len())
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
            .collect()
    }
}

impl fmt::Display for Board {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        // Horizontal indices
        write!(
            f,
            "  {}",
            self.indices
                .iter()
                .map(|&i| i.to_string())
                .collect::<Vec<String>>()
                .join(" ")
                .bold()
        )?;

        for y in &self.indices {
            // Vertical index
            write!(f, "\n{}", y.to_string().bold())?;
            // Row values
            for x in &self.indices {
                write!(
                    f,
                    " {}",
                    self.board[y * self.size + x].board_char_with_color()
                )?;
            }
        }

        Ok(())
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

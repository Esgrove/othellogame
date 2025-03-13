//! # Settings
//! Defines game settings
//!
//! Akseli Lukkarila
//! 2019-2025

use std::fmt;
use std::fmt::Display;

pub const MIN_BOARD_SIZE: usize = 4;
pub const MAX_BOARD_SIZE: usize = 10;
pub const DEFAULT_BOARD_SIZE: usize = 8;

/// Game settings.
#[derive(Debug, Copy, Clone)]
pub struct Settings {
    pub board_size: usize,
    pub autoplay_mode: bool,
    pub check_mode: bool,
    pub show_helpers: bool,
    pub show_log: bool,
    pub test_mode: bool,
    pub use_defaults: bool,
}

/// Player settings.
#[derive(Debug, Copy, Clone, PartialEq, Eq)]
pub struct PlayerSettings {
    pub show_helpers: bool,
    pub check_mode: bool,
    pub test_mode: bool,
}

impl Settings {
    /// Get player setting values from overall game settings.
    pub const fn to_player_settings(self) -> PlayerSettings {
        PlayerSettings {
            show_helpers: self.show_helpers,
            check_mode: self.check_mode,
            test_mode: self.test_mode,
        }
    }
}

impl Default for Settings {
    fn default() -> Self {
        Self {
            board_size: 8,
            check_mode: false,
            test_mode: false,
            autoplay_mode: false,
            use_defaults: false,
            show_helpers: true,
            show_log: false,
        }
    }
}

impl Default for PlayerSettings {
    fn default() -> Self {
        Self {
            show_helpers: true,
            check_mode: false,
            test_mode: false,
        }
    }
}

impl Display for Settings {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "\
Settings:
  board_size: {}
  autoplay: {}
  check_mode: {}
  use_defaults: {}
  show_helpers: {}
  show_log: {}
  test_mode: {}",
            self.board_size,
            self.autoplay_mode,
            self.check_mode,
            self.use_defaults,
            self.show_helpers,
            self.show_log,
            self.test_mode
        )
    }
}

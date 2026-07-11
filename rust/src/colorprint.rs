//! # Colorprint
//! Interface for coloured terminal printing.
//!
//! Akseli Lukkarila
//! 2019-2026

use colored::Colorize;

/// Print bold text with the given format string and arguments.
#[macro_export]
macro_rules! print_bold {
    ($($arg:tt)*) => {{
        use colored::Colorize;
        println!("{}", format!($($arg)*).bold())
    }};
}

/// Print text in green.
#[macro_export]
macro_rules! print_green {
    ($($arg:tt)*) => {{
        use colored::Colorize;
        println!("{}", format!($($arg)*).green())
    }};
}

/// Print bold text in green.
#[macro_export]
macro_rules! print_green_bold {
    ($($arg:tt)*) => {{
        use colored::Colorize;
        println!("{}", format!($($arg)*).green().bold())
    }};
}

/// Print text in yellow.
#[macro_export]
macro_rules! print_yellow {
    ($($arg:tt)*) => {{
        use colored::Colorize;
        println!("{}", format!($($arg)*).yellow())
    }};
}

/// Print bold text in yellow.
#[macro_export]
macro_rules! print_yellow_bold {
    ($($arg:tt)*) => {{
        use colored::Colorize;
        println!("{}", format!($($arg)*).yellow().bold())
    }};
}

/// Print text in red.
#[macro_export]
macro_rules! print_red {
    ($($arg:tt)*) => {{
        use colored::Colorize;
        println!("{}", format!($($arg)*).red())
    }};
}

/// Print bold text in red.
#[macro_export]
macro_rules! print_red_bold {
    ($($arg:tt)*) => {{
        use colored::Colorize;
        println!("{}", format!($($arg)*).red().bold())
    }};
}

/// Print error message with red colour.
pub fn print_error(message: &str) {
    let (indent, text) = split_leading_whitespace(message);
    println!("{}", format!("{indent}Error: {text}").red());
}

/// Print warning message with yellow colour.
pub fn print_warn(message: &str) {
    let (indent, text) = split_leading_whitespace(message);
    println!("{}", format!("{indent}Warning: {text}").yellow());
}

/// Split a string into the leading whitespace and the rest of the string.
fn split_leading_whitespace(message: &str) -> (&str, &str) {
    let indent_size = message.chars().take_while(|c| c.is_whitespace()).count();
    message.split_at(indent_size)
}

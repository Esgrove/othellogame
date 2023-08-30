//==========================================================
// Colorprint
// Interface for colored printing
// Akseli Lukkarila
// 2019-2023
//==========================================================

use colored::Colorize;

/// Print error message with red colour.
pub fn print_error(message: &str) {
    let indent_size = message.chars().take_while(|c| c.is_whitespace()).count();
    let (indent, text) = message.split_at(indent_size);
    println!("{}", format!("{}Error: {}", indent, text).red())
}

/// Print warning message with yellow colour.
pub fn print_warn(message: &str) {
    let indent_size = message.chars().take_while(|c| c.is_whitespace()).count();
    let (indent, text) = message.split_at(indent_size);
    println!("{}", format!("{}Warning: {}", indent, text).yellow())
}

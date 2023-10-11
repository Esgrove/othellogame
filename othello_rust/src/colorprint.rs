//==========================================================
// Colorprint
// Interface for colored printing
// Akseli Lukkarila
// 2019-2023
//==========================================================

use colored::Colorize;

/// Print error message with red colour.
pub fn print_error(message: &str) {
    let (indent, text) = split_leading_whitespace(message);
    println!("{}", format!("{}Error: {}", indent, text).red())
}

/// Print warning message with yellow colour.
pub fn print_warn(message: &str) {
    let (indent, text) = split_leading_whitespace(message);
    println!("{}", format!("{}Warning: {}", indent, text).yellow())
}

/// Split a string into the leading whitespace and the rest of the string.
fn split_leading_whitespace(message: &str) -> (&str, &str) {
    let indent_size = message.chars().take_while(|c| c.is_whitespace()).count();
    message.split_at(indent_size)
}

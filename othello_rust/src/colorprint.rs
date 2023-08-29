//==========================================================
// Colorprint
// Interface for colored printing
// Akseli Lukkarila
// 2019-2023
//==========================================================

use colored::Colorize;

pub fn print_error(message: &str) {
    let full_message = format!("Error: {}", message);
    println!("{}", full_message.red())
}

pub fn print_warn(message: &str) {
    let full_message = format!("Warning: {}", message);
    println!("{}", full_message.yellow())
}

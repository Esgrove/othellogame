//! # Utils
//! Helper utilities.
//!
//! Akseli Lukkarila
//! 2019-2025

use sha2::{Digest, Sha256};

/// Calculate SHA256 hash for the given string.
pub fn calculate_sha256(formatted_log: &String) -> String {
    let hash = Sha256::digest(formatted_log.as_bytes());
    base16ct::lower::encode_string(&hash)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_calculate_sha256_simple() {
        let test_string = String::from("test");
        let hash = calculate_sha256(&test_string);
        assert_eq!(
            hash,
            "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08"
        );
    }

    #[test]
    fn test_calculate_sha256_log_line() {
        let test_string = String::from("01: B:(0,1),1;____BBB__BW_____");
        let hash = calculate_sha256(&test_string);
        assert_eq!(
            hash,
            "d087cd890b81b7153cc870fba7372396e91cdcd4b6d13b82bbc88b5a4eb889d1"
        );
    }
    #[test]
    fn test_calculate_sha256_full_log() {
        let mut test_log = String::new();
        test_log.push_str("01: B:(0,1),1;____BBB__BW_____\n");
        test_log.push_str("02: W:(0,0),1;W___BWB__BW_____\n");
        test_log.push_str("03: B:(1,0),1;WB__BBB__BW_____\n");
        test_log.push_str("04: W:(0,2),2;WB__WBB_WWW_____\n");
        test_log.push_str("05: B:(0,3),1;WB__WBB_WBW_B___\n");
        test_log.push_str("06: W:(2,0),3;WWW_WWW_WBW_B___\n");
        test_log.push_str("07: B:(3,0),1;WWWBWWB_WBW_B___\n");
        test_log.push_str("08: W:(1,3),1;WWWBWWB_WWW_BW__\n");
        test_log.push_str("09: B:(2,3),2;WWWBWWB_WWB_BBB_\n");
        test_log.push_str("10: W:(3,2),2;WWWBWWW_WWWWBBB_\n");
        test_log.push_str("11: B:(3,1),1;WWWBWWWBWWBWBBB_\n");
        test_log.push_str("12: W:(3,3),1;WWWBWWWBWWWWBBBW");
        let hash = calculate_sha256(&test_log);
        assert_eq!(
            hash,
            "ad4679949110ed7997aa1bf09441a7eb491b498189c03685ffdd6ddbb70e3c22"
        );
    }
}

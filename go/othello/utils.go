//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
// 2019-2026
//==========================================================

package othello

import (
	"cmp"
	"crypto/sha256"
	"encoding/hex"
)

// CalculateSHA256 returns the SHA256 hash for the given string.
func CalculateSHA256(input string) string {
	data := []byte(input)
	hash := sha256.Sum256(data)
	hashString := hex.EncodeToString(hash[:])
	return hashString
}

// Clamp a value to the given range.
func Clamp[T cmp.Ordered](value T, minimum T, maximum T) T {
	if minimum > maximum {
		panic("Minimum value should be less than or equal to maximum!")
	}
	return max(minimum, min(value, maximum))
}

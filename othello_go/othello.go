package main

import "fmt"

// Ask and return the desired board size.
func getBoardSize() int {
	fmt.Print("Choose board size (default is 8): ")
	var input int
	_, err := fmt.Scanf("%d", &input)
	if err != nil {
		printWarn("Invalid value, defaulting to 8...")
		return 8
	} else {
		return clamp(input, 4, 10)
	}
}

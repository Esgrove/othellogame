package main

import (
	"fmt"

	"github.com/logrusorgru/aurora/v4"
)

func main() {
	fmt.Println("Hello,", aurora.Magenta("Aurora"))
	fmt.Println(aurora.Bold(aurora.Cyan("Cya!")))
}

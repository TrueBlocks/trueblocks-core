package usage

import (
	"bufio"
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/colors"
)

// queryUserHelper is a private helper function that handles user input
// defaultToYes determines what happens when the user hits enter with no input
func queryUserHelper(prompt, noResponse string, defaultToYes bool) bool {
	if os.Getenv("TB_NO_USERQUERY") == "true" {
		return true
	} else if len(os.Getenv("TB_NO_USERQUERY")) > 0 {
		return false
	}

	reader := bufio.NewReader(os.Stdin)
	fmt.Fprintf(os.Stderr, colors.Yellow+"%s"+colors.Off, prompt)
	text, _ := reader.ReadString('\n')
	text = strings.ReplaceAll(text, "\n", "")

	// Handle empty input (user just hit enter)
	if text == "" {
		return defaultToYes
	}

	// Handle explicit yes
	if text == "y" || text == "Y" {
		return true
	}

	// Handle quit
	text = strings.ToLower(text)
	if text == "q" || text == "quit" {
		fmt.Fprintf(os.Stderr, "Quitting...\n")
		os.Exit(0)
	}

	// Everything else is treated as no
	fmt.Fprintf(os.Stderr, "%s [%s]\n", noResponse, text)
	return false
}

// QueryUser prompts the user and defaults to "yes" when enter is pressed
func QueryUser(prompt, noResponse string) bool {
	return queryUserHelper(prompt, noResponse, true)
}

// QueryUserNo prompts the user and defaults to "no" when enter is pressed
func QueryUserNo(prompt, noResponse string) bool {
	return queryUserHelper(prompt, noResponse, false)
}

func Wait() {
	QueryUser("Press Enter to continue", "Continuing...")
}

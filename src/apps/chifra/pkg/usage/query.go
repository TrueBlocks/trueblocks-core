package usage

import (
	"bufio"
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
)

func QueryUser(prompt, noResponse string) bool {
	if os.Getenv("NO_USERQUERY") == "true" {
		return true
	}

	reader := bufio.NewReader(os.Stdin)
	fmt.Printf(colors.Yellow+"%s"+colors.Off, prompt)
	text, _ := reader.ReadString('\n')
	text = strings.Replace(text, "\n", "", -1)
	if text != "" && text != "y" && text != "Y" {
		text = strings.ToLower(text)
		if text == "q" || text == "quit" {
			fmt.Printf("Quitting...\n")
			os.Exit(0)
		}
		fmt.Printf("%s [%s]\n", noResponse, text)
		return false
	}
	return true
}

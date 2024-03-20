package main

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

func main() {
	// assumes it's running from ./build folder
	thePath := "../src/other/data-models/"
	pwd, _ := os.Getwd()
	fmt.Println(thePath, file.FolderExists(thePath), pwd)
}

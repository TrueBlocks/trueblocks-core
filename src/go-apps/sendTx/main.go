package main

import (
	"os"

	"github.com/Great-Hill-Corporation/trueblocks-core/src/go-apps/sendTx/cmd"
)

func main() {
	cmd.Execute()
	os.Exit(0)
}

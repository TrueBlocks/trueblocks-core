package file

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func NFilesInFolder(path string) int {
	files, err := os.ReadDir(path)
	if err != nil {
		logger.Fatal(err)
	}
	return len(files)
}

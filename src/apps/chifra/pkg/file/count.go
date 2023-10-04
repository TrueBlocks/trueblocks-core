package file

import (
	"os"
)

func NFilesInFolder(path string) int {
	files, _ := os.ReadDir(path)
	return len(files)
}

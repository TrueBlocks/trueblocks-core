package file

import (
	"log"
	"os"
)

func NFilesInFolder(path string) int {
	files, err := os.ReadDir(path)
	if err != nil {
		log.Fatal(err)
	}
	return len(files)
}

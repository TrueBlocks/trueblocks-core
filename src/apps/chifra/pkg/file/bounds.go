package file

import (
	"fmt"
	"os"
	"path/filepath"
)

// LatestFileInFolder returns the latest (alphabetically) file in the folder
func LatestFileInFolder(path string) (string, error) {
	files, err := os.ReadDir(path)
	if err != nil {
		return "", err
	}
	if len(files) == 0 {
		return "", fmt.Errorf("No files found in folder " + path)
	}
	return filepath.Join(path, files[len(files)-1].Name()), nil
}

// EarliestFileInFolder returns the first (alphabetically) file in the folder
func EarliestFileInFolder(path string) (string, error) {
	files, err := os.ReadDir(path)
	if err != nil {
		return "", err
	}
	if len(files) == 0 {
		return "", fmt.Errorf("No files found in folder " + path)
	}
	return filepath.Join(path, files[0].Name()), nil
}

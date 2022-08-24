package file

import (
	"os"
	"path/filepath"
	"strings"
)

func MakeBackup(tmpPath, origFn string) (string, error) {
	if !FileExists(origFn) {
		return "", nil
	}

	_, name := filepath.Split(origFn)
	pattern := strings.Replace(name, ".", ".*.", -1)
	tmpFile, err := os.CreateTemp(tmpPath, pattern)
	if err != nil {
		return "", err
	}
	tmpFile.Close()
	tmpFn := tmpFile.Name()
	_, err = Copy(tmpFn, origFn)
	return tmpFn, err
}

package file

import (
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func MakeBackup(origFn, path string) (string, error) {
	if !FileExists(origFn) {
		return "", nil
	}

	_, name := filepath.Split(origFn)
	logger.Log(logger.Info, colors.BrightGreen, "name:", name, colors.Off)
	pattern := strings.Replace(name, ".", ".*.", -1)
	logger.Log(logger.Info, colors.BrightGreen, "pattern:", pattern, colors.Off)
	tmpFile, err := os.CreateTemp(path, pattern)
	logger.Log(logger.Info, colors.BrightGreen, "tmpFile:", tmpFile.Name(), colors.Off)
	if err != nil {
		return "", err
	}
	tmpFile.Close()
	tmpFn := tmpFile.Name()
	_, err = Copy(tmpFn, origFn)
	return tmpFn, err
}

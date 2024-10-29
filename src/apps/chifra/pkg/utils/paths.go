package utils

import (
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// GetConfigFn returns the user's (OS-specific) configuration folder. If the folder
// is not found, an error is returned. If appDir is not empty, it is appended to
// the configDir and if the resulting folder does not exist, it is created.
func GetConfigFn(appDir, fn string) (string, error) {
	if userConfig, err := os.UserConfigDir(); err != nil {
		return "", err
	} else {
		configFolder := filepath.Join(userConfig, "TrueBlocks", appDir)
		if !file.FolderExists(configFolder) {
			_ = file.EstablishFolder(configFolder)
		}
		return filepath.Join(configFolder, fn), nil
	}
}

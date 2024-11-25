package utils

import (
	"errors"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// MustGetConfigFn ignore errors on GetConfigFn
func MustGetConfigFn(appDir, fn string) string {
	ret, _ := GetConfigFn(appDir, fn)
	return ret
}

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

// GetCacheDir returns the user's (OS-specific) caching folder. If the folder
// is not found, an error is returned. If appDir is not empty, it is appended to
// the cacheDir and if the resulting folder does not exist, it is created.
func GetCacheDir(appDir string) (string, error) {
	if cachePath, err := os.UserCacheDir(); err != nil {
		return "", err
	} else {
		path := filepath.Join(cachePath, appDir)
		if !file.FolderExists(path) {
			_ = file.EstablishFolder(path)
		}
		return path, nil
	}
}

var ErrDocFolderNotFound = errors.New("no documents folder found")

// GetDocumentsDir returns the user's documents folder. The documents folder is
// the user's (OS-specific) home folder with the word "Documents" appended to it.
// If the home folder or the $HOME/Documents folder is not found, an error is returned.
func GetDocumentsDir() (string, error) {
	if userPath, err := os.UserHomeDir(); err != nil {
		return "", err
	} else {
		path := filepath.Join(userPath, "Documents")
		if !file.FolderExists(path) {
			return "", ErrDocFolderNotFound
		}
		return path, nil
	}
}

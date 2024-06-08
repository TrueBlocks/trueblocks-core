package file

import (
	"os"
	"path/filepath"
	"strings"
)

type BackupFile struct {
	OrigFn   string
	BackupFn string
}

// Restore restores the original file from the backup file.
func (bf *BackupFile) Restore() {
	if !FileExists(bf.BackupFn) {
		// The caller was successful. Nothing to clean up.
		return
	} else {
		// The backup remains, something went wrong, so we replace the original file.
		_ = os.Rename(bf.BackupFn, bf.OrigFn)
		// This may seem redundant, but it's not on some operating systems
		_ = os.Remove(bf.BackupFn)
	}
}

// Clear removes the backup file.
func (bf *BackupFile) Clear() {
	if FileExists(bf.BackupFn) {
		_ = os.Remove(bf.BackupFn)
	}
}

// MakeBackup creates a backup of the original file and returns a structure that
// can be used to restore the original file in case of an error.
func MakeBackup(tmpPath, origFn string) (BackupFile, error) {
	if !FileExists(origFn) {
		return BackupFile{}, nil
	}

	_, name := filepath.Split(origFn)
	pattern := strings.Replace(name, ".", ".*.", -1)
	tmpFile, err := os.CreateTemp(tmpPath, pattern)
	if err != nil {
		return BackupFile{}, err
	}
	tmpFile.Close()
	tmpFn := tmpFile.Name()
	_, err = Copy(tmpFn, origFn)
	return BackupFile{
		OrigFn:   origFn,
		BackupFn: tmpFn,
	}, err
}

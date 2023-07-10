package locations

import (
	"errors"
	"io"
	"os"
	"path/filepath"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// Each Storer implementation is global and thread-safe to save resources
// when reading/writing large number of items
var fsInstance *fileSystem
var fsInitOnce sync.Once

type fileSystem struct{}

// FileSystem returns an instance of fileSystem Storer, ready to be used
func FileSystem() (*fileSystem, error) {
	fsInitOnce.Do(func() {
		fsInstance = new(fileSystem)
	})
	return fsInstance, nil
}

// Writer returns io.WriterCloser for the item at given path
func (l *fileSystem) Writer(path string) (io.WriteCloser, error) {
	// TODO: add file locks
	// TODO: add mkdir
	file, err := os.OpenFile(path, os.O_WRONLY|os.O_TRUNC|os.O_CREATE, 0666)
	if err != nil {
		return nil, err
	}
	return file, nil
}

// Reader returns io.ReaderCloser for the item at given path
func (l *fileSystem) Reader(path string) (io.ReadCloser, error) {
	return nil, errors.New("not implemented")
}

// Remove removes the item at given path
func (l *fileSystem) Remove(path string) error {
	if err := os.Remove(path); err != nil {
		return err
	}

	dirPath, _ := filepath.Split(path)
	if empty, _ := file.IsFolderEmpty(dirPath); empty {
		// we ignore the error
		os.RemoveAll(dirPath)
	}
	return nil
}

func (l *fileSystem) Stat(path string) (*ItemInfo, error) {
	info, err := os.Stat(path)
	if err != nil {
		return nil, err
	}

	return &ItemInfo{
		fileSize: int(info.Size()),
	}, err
}

package locations

import (
	"errors"
	"io"
	"os"
	"path/filepath"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
)

const FS_PERMISSIONS = 0755

// Each Storer implementation is global and thread-safe to save resources
// when reading/writing large number of items
var fsInstance *fileSystem
var fsInitOnce sync.Once

// fsReadWriteCloser is a wrapper around os.File that removes any file locks
// when Close() is called
type fsReadWriteCloser struct {
	*os.File
}

// Close closes the underlying os.File and removes file lock
func (f *fsReadWriteCloser) Close() error {
	// We unlock file when the caller is done
	if err := file.Unlock(f.File); err != nil {
		return err
	}
	return f.File.Close()
}

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
	if err := l.makeParentDirectories(path); err != nil {
		return nil, err
	}

	output, err := file.WaitOnLock(path, os.O_WRONLY|os.O_TRUNC|os.O_CREATE)
	if err != nil {
		return nil, err
	}

	return &fsReadWriteCloser{output}, nil
}

// Reader returns io.ReaderCloser for the item at given path
func (l *fileSystem) Reader(path string) (io.ReadCloser, error) {
	input, err := file.WaitOnLock(path, os.O_RDWR)
	if err != nil {
		if errors.Is(err, os.ErrNotExist) {
			return nil, ErrNotFound
		}
		return nil, err
	}

	return &fsReadWriteCloser{input}, nil
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
		name:     info.Name(),
	}, err
}

func (l *fileSystem) makeParentDirectories(path string) error {
	dirPath, _ := filepath.Split(path)
	return os.MkdirAll(dirPath, FS_PERMISSIONS)
}

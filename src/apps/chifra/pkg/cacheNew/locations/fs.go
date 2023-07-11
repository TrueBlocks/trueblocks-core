package locations

import (
	"context"
	"errors"
	"io"
	"os"
	"path/filepath"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

const FS_PERMISSIONS = 0644

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
func (l *fileSystem) Writer(ctx context.Context, path string) (io.WriteCloser, error) {
	if err := l.makeParentDirectories(path); err != nil {
		return nil, err
	}

	// output, err := os.OpenFile(path, os.O_WRONLY|os.O_TRUNC|os.O_CREATE, FS_PERMISSIONS)
	output, err := file.WaitOnLock(path, os.O_WRONLY|os.O_TRUNC|os.O_CREATE)
	if err != nil {
		return nil, err
	}
	// if err := file.Lock(output); err != nil {
	// 	return nil, err
	// }

	// We unlock file when the caller is done
	go func() {
		<-ctx.Done()
		file.Unlock(output)
	}()

	return output, nil
}

// Reader returns io.ReaderCloser for the item at given path
func (l *fileSystem) Reader(ctx context.Context, path string) (io.ReadCloser, error) {
	if err := l.makeParentDirectories(path); err != nil {
		return nil, err
	}

	// input, err := os.OpenFile(path, os.O_RDONLY|os.O_CREATE, FS_PERMISSIONS)
	input, err := file.WaitOnLock(path, os.O_RDWR)
	if err != nil {
		if errors.Is(err, os.ErrNotExist) {
			return nil, ErrNotFound
		}
		return nil, err
	}
	// if err := file.WaitOnLock(path, os.O_RDONLY); err != nil {
	// 	return nil, err
	// }

	// We unlock file when the caller is done
	go func() {
		<-ctx.Done()
		file.Unlock(input)
	}()

	return input, nil
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

func (l *fileSystem) makeParentDirectories(path string) error {
	dirPath, _ := filepath.Split(path)
	return os.MkdirAll(dirPath, FS_PERMISSIONS)
}

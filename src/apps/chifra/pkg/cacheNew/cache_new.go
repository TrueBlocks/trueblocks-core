package cacheNew

import (
	"errors"
	"io"
	"path"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew/locations"
)

type StoreLocation uint

const (
	FsCache StoreLocation = iota
	IpfsCache
	MemoryCache
)

type Storer interface {
	Writer(path string) (io.WriteCloser, error)
	Reader(path string) (io.ReadCloser, error)
	Remove(path string) error
	Stat(path string) (*locations.ItemInfo, error)
}

type Locator interface {
	CacheLocation() (directory string, extension string)
	CacheId() string
	CacheName() string
}

type StoreOptions struct {
	Location StoreLocation
}

func (s *StoreOptions) location() (loc Storer, err error) {
	if s == nil {
		loc, err = locations.FileSystem()
		return
	}
	switch s.Location {
	case MemoryCache:
		loc, err = locations.Memory()
	case IpfsCache:
		// TODO: use ipfs
		return nil, errors.New("not implemented")
	case FsCache:
		fallthrough
	default:
		loc, err = locations.FileSystem()
	}

	return
}

func cachePath(value Locator, id string) (locPath string, err error) {
	if id == "" {
		id = value.CacheId()
	}
	directory, extension := value.CacheLocation()
	if id == "" {
		err = errors.New("empty CacheId")
		return
	}
	if directory == "" || extension == "" {
		err = errors.New("empty CacheLocation")
		return
	}
	locPath = path.Join(directory, (id + "." + extension))
	return
}

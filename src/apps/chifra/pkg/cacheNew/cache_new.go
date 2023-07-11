package cacheNew

import (
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew/locations"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

type StoreLocation uint

const (
	FsCache StoreLocation = iota
	// IpfsCache
	MemoryCache
)

type Storer interface {
	// Writer returns io.WriteCloser for the given cache item
	Writer(path string) (io.WriteCloser, error)
	// Reader returns io.ReaderCloser for the given cache item
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
	Chain    string
	// Optional
	RootDir string
}

func (s *StoreOptions) location() (loc Storer, err error) {
	if s == nil {
		loc, err = locations.FileSystem()
		return
	}
	switch s.Location {
	case MemoryCache:
		loc, err = locations.Memory()
	// case IpfsCache:
	// 	// TODO: use ipfs
	// 	return nil, errors.New("not implemented")
	case FsCache:
		fallthrough
	default:
		loc, err = locations.FileSystem()
	}

	return
}

func (s *StoreOptions) rootDir() (dir string) {
	if s == nil {
		dir = config.GetPathToCache(config.GetDefaultChain())
	} else if s.RootDir == "" {
		dir = config.GetPathToCache(s.Chain)
	}
	if dir != "" {
		// TODO: v1 suffix
		return dir + "/v1"
	}
	return s.RootDir
}

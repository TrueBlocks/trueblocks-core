package cacheNew

import (
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew/locations"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

type StoreLocation uint

const (
	FsCache StoreLocation = iota
	MemoryCache
)

// Storer stores items in the given location. Cache is agnostic
// of the actual location of the items (files in FS terms) and
// Storer is responsible for all low-level work.
type Storer interface {
	// Writer returns io.WriteCloser for the given cache item
	Writer(path string) (io.WriteCloser, error)
	// Reader returns io.ReaderCloser for the given cache item
	Reader(path string) (io.ReadCloser, error)
	Remove(path string) error
	Stat(path string) (*locations.ItemInfo, error)
}

// A struct implementing Locator can describe its location in the
// cache
type Locator interface {
	CacheLocation() (directory string, extension string)
	CacheId() string
	CacheName() string
}

// A struct implementing Unmarshaler can be read from binary by
// calling UnmarshalCache
type Unmarshaler interface {
	UnmarshalCache(version uint64, reader io.Reader) error
}

// A struct implementing Marshaler can be written to binary by
// calling MarshalCache
type Marshaler interface {
	MarshalCache(writer io.Writer) error
}

// Options used by Store
type StoreOptions struct {
	Location StoreLocation
	Chain    string

	// Optional

	RootDir string
	// If ReadOnly is true, then we will not write to the cache
	ReadOnly bool
}

func (s *StoreOptions) location() (loc Storer, err error) {
	if s == nil {
		loc, err = locations.FileSystem()
		return
	}
	switch s.Location {
	case MemoryCache:
		loc, err = locations.Memory()
	case FsCache:
		fallthrough
	default:
		loc, err = locations.FileSystem()
	}

	return
}

func (s *StoreOptions) rootDir() (dir string) {
	if s != nil && s.Location == MemoryCache {
		return "memory"
	}

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

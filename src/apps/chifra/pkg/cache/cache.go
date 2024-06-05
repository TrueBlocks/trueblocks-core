package cache

import (
	"io"
	"log"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache/locations"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

type StoreLocation uint

const (
	FsCache StoreLocation = iota
	MemoryCache
)

// NoCache indicates that we are not caching or reading from the cache
var NoCache *Store = nil

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

// Locator is a struct implementing the Locator interface. It can describe its
// location in the cache
type Locator interface {
	CacheLocation() (directory string, extension string)
	CacheId() string
	CacheName() string
}

// Unmarshaler is a struct implementing Unmarshaler can be read from binary by
// calling UnmarshalCache
type Unmarshaler interface {
	UnmarshalCache(vers uint64, reader io.Reader) error
}

// Marshaler is a struct implementing the Marshaler interface. It can be
// written to binary by calling MarshalCache
type Marshaler interface {
	MarshalCache(writer io.Writer) error
}

// StoreOptions used by Store
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
		log.Fatal("should not happen ==> implementation error in location.")
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
		log.Fatal("should not happen ==> implementation error in location.")
	} else if s.RootDir == "" {
		dir = config.PathToCache(s.Chain)
	}

	if dir != "" {
		// TODO: v1 suffix
		return dir + "/v1"
	}

	return s.RootDir
}

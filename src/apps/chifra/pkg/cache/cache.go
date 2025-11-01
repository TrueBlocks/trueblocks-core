package cache

import (
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/cache/locations"
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
	CacheLocations() (string, string, string)
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

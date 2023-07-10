package cacheNew

import (
	"bytes"
	"context"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew/locations"
)

var defaultStore *Store
var defaultStoreOnce sync.Once

type Store struct {
	resolvedPaths map[Locator]string
	location      Storer
}

func NewStore(options *StoreOptions) (*Store, error) {
	location, err := options.location()
	if err != nil {
		return nil, err
	}
	return &Store{
		location: location,
	}, nil
}

func DefaultStore() (*Store, error) {
	var err error
	defaultStoreOnce.Do(func() {
		defaultStore, err = NewStore(nil)
	})
	return defaultStore, err
}

func (s *Store) resolvePath(value Locator, id string) (string, error) {
	if path, ok := s.resolvedPaths[value]; ok {
		return path, nil
	}
	return cachePath(value, id)
}

// Write options that we might need in the future
type WriteOptions interface{}

// Write saves value to a location defined by options.Location. If options is nil,
// then FileSystem is used. The value has to implement Locator interface, which
// provides information about in-cache path and ID.
func (s *Store) Write(value Locator, options *WriteOptions) (err error) {
	itemPath, err := s.resolvePath(value, "")
	if err != nil {
		return
	}

	ctx, done := context.WithCancel(context.Background())
	defer done()

	writer, err := s.location.Writer(ctx, itemPath)
	if err != nil {
		return
	}
	defer writer.Close()

	buffer := new(bytes.Buffer)
	item := NewItem(buffer)
	if err = item.Encode(value); err != nil {
		return
	}
	_, err = buffer.WriteTo(writer)

	return
}

// Read options that we might need in the future
type ReadOptions interface{}

// Read retrieves value of ID id from a location defined by options.Location. If options is nil,
// then FileSystem is used. The value has to implement Locator interface, which
// provides information about in-cache path
func (s *Store) Read(value Locator, id string, options *ReadOptions) (err error) {
	itemPath, err := s.resolvePath(value, id)
	if err != nil {
		return
	}

	ctx, done := context.WithCancel(context.Background())
	defer done()

	reader, err := s.location.Reader(ctx, itemPath)
	if err != nil {
		return
	}
	defer reader.Close()

	buffer := new(bytes.Buffer)
	if _, err = buffer.ReadFrom(reader); err != nil {
		return
	}

	item := NewItem(buffer)
	return item.Decode(value)
}

func (s *Store) Stat(value Locator) (result *locations.ItemInfo, err error) {
	itemPath, err := s.resolvePath(value, "")
	if err != nil {
		return
	}
	return s.location.Stat(itemPath)
}

func (s *Store) Remove(value Locator) error {
	itemPath, err := s.resolvePath(value, "")
	if err != nil {
		return err
	}
	return s.location.Remove(itemPath)
}

type DecacheFunc func(*locations.ItemInfo) bool

// TODO: chain
func (s *Store) Decache(locators []Locator, processor DecacheFunc) (err error) {
	for _, locator := range locators {
		// directory, extension := locator.CacheLocation()
		// cachePath := path.Join(directory, locator.CacheId(), extension)
		stats, err := s.Stat(locator)
		if err != nil {
			return err
		}
		// If processor returns false, we don't want to remove this item from the cache
		if !processor(stats) {
			continue
		}
		if err := s.Remove(locator); err != nil {
			return err
		}
	}

	return nil
}

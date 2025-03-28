package cache

import (
	"bytes"
	"context"
	"errors"
	"os"
	"path/filepath"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache/locations"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/sigintTrap"
	"github.com/ethereum/go-ethereum/log"
)

var ErrReadOnly = errors.New("cache is read-only")
var ErrCanceled = errors.New("write canceled")

var defaultStore *Store
var defaultStoreOnce sync.Once

// Store holds all information necessary to access the cache, no matter
// which concrete location (FS, IPFS, memory, etc.) is being used
type Store struct {
	resolvedPaths map[Locator]string
	location      Storer
	rootDir       string
	// If readOnly is true, Store will not write to the cache, but
	// still read (issue #3047)
	readOnly bool
}

func NewStore(options *StoreOptions) (*Store, error) {
	location, err := options.location()
	if err != nil {
		return nil, err
	}
	return &Store{
		location: location,
		rootDir:  options.rootDir(),
		readOnly: options.ReadOnly,
	}, nil
}

func DefaultStore() (*Store, error) {
	var err error
	defaultStoreOnce.Do(func() {
		defaultStore, err = NewStore(nil)
	})
	return defaultStore, err
}

func (s *Store) resolvePath(value Locator) (resolved string, err error) {
	if cachedPath, ok := s.resolvedPaths[value]; ok {
		return cachedPath, nil
	}

	directory, id, extension := value.CacheLocations()
	if directory == "" || extension == "" {
		err = errors.New("empty CacheLocations")
		return
	}
	if filepath.IsAbs(directory) {
		resolved = filepath.Join(directory, (id + "." + extension))
	} else {
		resolved = filepath.Join(s.rootDir, directory, (id + "." + extension))
	}

	return
}

// WriteOptions passes additional context to Write if needed
type WriteOptions interface{}

// Write saves value to a location defined by options.Location. If options is nil,
// then FileSystem is used. The value has to implement Locator interface, which
// provides information about in-cache path and ID.
func (s *Store) Write(value Locator, options *WriteOptions) (err error) {
	_ = options
	if s.readOnly {
		err = ErrReadOnly
		return
	}

	itemPath, err := s.resolvePath(value)
	if err != nil {
		return
	}

	ctx, cancel := context.WithCancel(context.Background())
	cleanOnQuit := func() {
		log.Warn(sigintTrap.TrapMessage)
	}
	trapChannel := sigintTrap.Enable(ctx, cancel, cleanOnQuit)
	defer sigintTrap.Disable(trapChannel)

	writer, err := s.location.Writer(itemPath)
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

// ReadOptions are options that we might need in the future
type ReadOptions interface{}

// Read retrieves value from a location defined by options.Location. If options is nil,
// then FileSystem is used. The value has to implement Locator interface, which
// provides information about in-cache path
func (s *Store) Read(value Locator, options *ReadOptions) (err error) {
	_ = options
	if itemPath, err := s.resolvePath(value); err != nil {
		return err
	} else {
		if reader, err := s.location.Reader(itemPath); err != nil {
			return err
		} else {
			defer reader.Close()
			buffer := new(bytes.Buffer)
			if _, err = buffer.ReadFrom(reader); err != nil {
				return err
			}
			item := NewItem(buffer)
			err = item.Decode(value)
			if err != nil {
				_ = os.Remove(itemPath)
			}
			return err
		}
	}
}

func (s *Store) Stat(value Locator) (*locations.ItemInfo, error) {
	if itemPath, err := s.resolvePath(value); err != nil {
		return nil, err
	} else {
		return s.location.Stat(itemPath)
	}
}

func (s *Store) Remove(value Locator) error {
	if itemPath, err := s.resolvePath(value); err != nil {
		return err
	} else {
		return s.location.Remove(itemPath)
	}
}

func (s *Store) Decache(locators []Locator, procFunc, skipFunc func(*locations.ItemInfo) bool) error {
	// TODO: The statements cache is unique in that it will store multiple versions of a statement
	// TODO: file for the same holder address. If there is no filter when the cache is created,
	// TODO: the cache will store the statements at holder-0-bn-txid. If there is a filter, it will
	// TODO: store at holder-asset-bn-txid. The code that calls this code will only send in the
	// TODO: first version leaving all others in place.
	for _, locator := range locators {
		if stats, err := s.Stat(locator); err != nil {
			// many locations will not have been cached, but we want to report
			skipFunc(stats)
			continue
		} else {
			// If processor returns false, we don't want to remove this item from the cache
			if !procFunc(stats) {
				continue
			}
			if err := s.Remove(locator); err != nil {
				return err
			}
		}
	}

	return nil
}

func (s *Store) ReadOnly() bool {
	return s.readOnly
}

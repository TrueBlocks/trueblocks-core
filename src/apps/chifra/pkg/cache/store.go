package cache

import (
	"bytes"
	"context"
	"errors"
	"fmt"
	"log"
	"os"
	"path/filepath"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache/locations"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/sigintTrap"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

var ErrReadOnly = errors.New("cache is read-only")
var ErrCanceled = errors.New("write canceled")

// Store holds all information necessary to access the cache, no matter
// which concrete location (FS, IPFS, memory, etc.) is being used
type Store struct {
	resolvedPaths map[Locator]string
	location      Storer
	rootDir       string
	enabled       bool
	enabledMap    map[walk.CacheType]bool
	latest        base.Timestamp
}

// StoreOptions used by Store
type StoreOptions struct {
	Chain      string
	Location   StoreLocation
	RootDir    string
	Enabled    bool
	EnabledMap map[walk.CacheType]bool
	Latest     base.Timestamp
}

func NewStore(options *StoreOptions) (*Store, error) {
	if options == nil {
		logger.Fatal("implementation error in NewStore - should never happen")
	}
	location, err := options.location()
	if err != nil {
		return nil, err
	}
	enabledMap := make(map[walk.CacheType]bool)
	if options.EnabledMap != nil {
		enabledMap = options.EnabledMap
	}
	return &Store{
		location:      location,
		rootDir:       options.rootDir(),
		enabled:       options.Enabled || options.Location == MemoryCache,
		enabledMap:    enabledMap,
		resolvedPaths: make(map[Locator]string),
	}, nil
}

func (s *Store) SetLatest(ts base.Timestamp) {
	s.latest = ts
}

func (s *Store) IsFinal(ts base.Timestamp) bool {
	return base.IsFinal(s.latest, ts)
}

func (s *Store) resolvePath(value Locator) (string, error) {
	if cachedPath, ok := s.resolvedPaths[value]; ok {
		return cachedPath, nil
	}
	directory, id, extension := value.CacheLocations()
	if directory == "" || id == "" || extension == "" {
		return "", errors.New("empty CacheLocations")
	}
	if filepath.IsAbs(directory) {
		return filepath.Join(directory, (id + "." + extension)), nil
	} else {
		return filepath.Join(s.rootDir, directory, (id + "." + extension)), nil
	}
}

// Write saves value to a location defined by options.Location. If options is nil,
// then FileSystem is used. The value has to implement Locator interface, which
// provides information about in-cache path and ID.
func (s *Store) Write(value Locator) error {
	if !s.enabled {
		return ErrReadOnly
	}
	if itemPath, err := s.resolvePath(value); err != nil {
		return err
	} else {
		ctx, cancel := context.WithCancel(context.Background())
		cleanOnQuit := func() {
			logger.Warn(sigintTrap.TrapMessage)
		}
		trapChannel := sigintTrap.Enable(ctx, cancel, cleanOnQuit)
		defer sigintTrap.Disable(trapChannel)

		if writer, err := s.location.Writer(itemPath); err != nil {
			return err
		} else {
			defer writer.Close()
			buffer := new(bytes.Buffer)
			item := NewItem(buffer)
			if err = item.Encode(value); err != nil {
				return err
			}
			_, err = buffer.WriteTo(writer)
			return err
		}
	}
}

// Read retrieves value from a location defined by options.Location. If options is nil,
// then FileSystem is used. The value has to implement Locator interface, which
// provides information about in-cache path
func (s *Store) Read(value Locator) error {
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

func (s *Store) Enabled() bool {
	return s != nil && s.enabled
}

// TestLog prints the enabledMap to the log. Note this routine gets called prior to full initialization, thus it takes the enabledMap
func (s *Store) TestLog() {
	if s.Enabled() {
		enabled := []string{}
		for k, v := range s.enabledMap {
			if v {
				enabled = append(enabled, k.String())
			}
		}
		sort.Strings(enabled)
		logger.TestLog(len(enabled) > 0, "Enabled: ", strings.Join(enabled, ", "))
	}
}

// WriteToStore handles caching of any data type that implements the Locator interface. Precondition: Caller
// must ensure caching is enabled and provide all conditions (e.g., isFinal, isWritable).
func (s *Store) WriteToStore(data Locator, cacheType walk.CacheType, ts base.Timestamp, conditions ...bool) error {
	if s == nil || !s.Enabled() || !s.enabledMap[cacheType] || !s.IsFinal(ts) {
		return nil
	}
	for _, cond := range conditions {
		if !cond {
			return nil
		}
	}
	if err := s.Write(data); err != nil {
		return fmt.Errorf("failed to write %s to cache: %w", cacheType.String(), err)
	}
	return nil
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
		return filepath.Join(dir, "v1")
	}

	return s.RootDir
}

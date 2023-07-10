package cacheNew

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew/locations"

type DecacheFunc func(*locations.ItemInfo) bool

// TODO: chain
func Decache(locators []Locator, processor DecacheFunc, options *StoreOptions) (err error) {
	for _, locator := range locators {
		// directory, extension := locator.CacheLocation()
		// cachePath := path.Join(directory, locator.CacheId(), extension)
		stats, err := Stat(locator, options)
		if err != nil {
			return err
		}
		// If processor returns false, we don't want to remove this item from the cache
		if !processor(stats) {
			continue
		}
		if err := Remove(locator, options); err != nil {
			return err
		}
	}

	return nil
}

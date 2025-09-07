package types

type CacheItemField string

// Fields in the CacheItem struct available for sorting.
const (
	CacheItemLastCached  CacheItemField = "lastCached"
	CacheItemNFiles      CacheItemField = "nFiles"
	CacheItemNFolders    CacheItemField = "nFolders"
	CacheItemPath        CacheItemField = "path"
	CacheItemSizeInBytes CacheItemField = "sizeInBytes"
	CacheItemType        CacheItemField = "type"
)

// GetSortFieldsCacheItem returns a []string of valid sort fields for the type.
func GetSortFieldsCacheItem() []string {
	return []string{"lastCached", "nFiles", "nFolders", "path", "sizeInBytes", "type"}
}

// CacheItemBy returns a comparison function for sorting CacheItem instances by the given field.
// These comparison functions may be strung together by the CmdCacheItems function.
func CacheItemBy(field CacheItemField, order SortOrder) func(p1, p2 CacheItem) bool {
	switch field {
	case CacheItemLastCached: // string
		return func(p1, p2 CacheItem) bool {
			if order == Ascending {
				return p1.LastCached < p2.LastCached
			}
			return p1.LastCached > p2.LastCached
		}
	case CacheItemNFiles: // uint64
		return func(p1, p2 CacheItem) bool {
			if order == Ascending {
				return p1.NFiles < p2.NFiles
			}
			return p1.NFiles > p2.NFiles
		}
	case CacheItemNFolders: // uint64
		return func(p1, p2 CacheItem) bool {
			if order == Ascending {
				return p1.NFolders < p2.NFolders
			}
			return p1.NFolders > p2.NFolders
		}
	case CacheItemPath: // string
		return func(p1, p2 CacheItem) bool {
			if order == Ascending {
				return p1.Path < p2.Path
			}
			return p1.Path > p2.Path
		}
	case CacheItemSizeInBytes: // int64
		return func(p1, p2 CacheItem) bool {
			if order == Ascending {
				return p1.SizeInBytes < p2.SizeInBytes
			}
			return p1.SizeInBytes > p2.SizeInBytes
		}
	case CacheItemType: // string
		return func(p1, p2 CacheItem) bool {
			if order == Ascending {
				return p1.CacheItemType < p2.CacheItemType
			}
			return p1.CacheItemType > p2.CacheItemType
		}

	}
	logger.ShouldNotHappen("in CacheItemBy")
	return nil
}

// CacheItemCmp accepts a slice and variadic comparison functions and returns a functions
// that can be used to sort the slice.
func CacheItemCmp(slice []CacheItem, orders ...func(p1, p2 CacheItem) bool) func(i, j int) bool {
	return func(i, j int) bool {
		p1, p2 := slice[i], slice[j]
		for _, order := range orders {
			if order(p1, p2) {
				return true
			}
			if order(p2, p1) {
				return false
			}
		}
		return false
	}
}

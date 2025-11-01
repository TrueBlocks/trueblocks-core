package types

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"

type AbiField string

// Fields in the Abi struct available for sorting.
const (
	AbiAddress        AbiField = "address"
	AbiFileSize       AbiField = "fileSize"
	AbiHasConstructor AbiField = "hasConstructor"
	AbiHasFallback    AbiField = "hasFallback"
	AbiIsEmpty        AbiField = "isEmpty"
	AbiIsKnown        AbiField = "isKnown"
	AbiLastModDate    AbiField = "lastModDate"
	AbiNEvents        AbiField = "nEvents"
	AbiNFunctions     AbiField = "nFunctions"
	AbiName           AbiField = "name"
)

// GetSortFieldsAbi returns a []string of valid sort fields for the type.
func GetSortFieldsAbi() []string {
	return []string{"address", "fileSize", "hasConstructor", "hasFallback", "isEmpty", "isKnown", "lastModDate", "nEvents", "nFunctions", "name"}
}

// AbiBy returns a comparison function for sorting Abi instances by the given field.
// These comparison functions may be strung together by the CmdAbis function.
func AbiBy(field AbiField, order SortOrder) func(p1, p2 Abi) bool {
	switch field {
	case AbiAddress: // address
		return func(p1, p2 Abi) bool {
			cmp := p1.Address.Cmp(p2.Address.Address)
			if order == Ascending {
				return cmp == -1
			}
			return cmp == 1
		}
	case AbiFileSize: // int64
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return p1.FileSize < p2.FileSize
			}
			return p1.FileSize > p2.FileSize
		}
	case AbiHasConstructor: // bool
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return !p1.HasConstructor && p2.HasConstructor
			}
			return p1.HasConstructor && !p2.HasConstructor
		}
	case AbiHasFallback: // bool
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return !p1.HasFallback && p2.HasFallback
			}
			return p1.HasFallback && !p2.HasFallback
		}
	case AbiIsEmpty: // bool
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return !p1.IsEmpty && p2.IsEmpty
			}
			return p1.IsEmpty && !p2.IsEmpty
		}
	case AbiIsKnown: // bool
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return !p1.IsKnown && p2.IsKnown
			}
			return p1.IsKnown && !p2.IsKnown
		}
	case AbiLastModDate: // string
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return p1.LastModDate < p2.LastModDate
			}
			return p1.LastModDate > p2.LastModDate
		}
	case AbiNEvents: // int64
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return p1.NEvents < p2.NEvents
			}
			return p1.NEvents > p2.NEvents
		}
	case AbiNFunctions: // int64
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return p1.NFunctions < p2.NFunctions
			}
			return p1.NFunctions > p2.NFunctions
		}
	case AbiName: // string
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return p1.Name < p2.Name
			}
			return p1.Name > p2.Name
		}

	}
	logger.ShouldNotHappen("in AbiBy")
	return nil
}

// AbiCmp accepts a slice and variadic comparison functions and returns a functions
// that can be used to sort the slice.
func AbiCmp(slice []Abi, orders ...func(p1, p2 Abi) bool) func(i, j int) bool {
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

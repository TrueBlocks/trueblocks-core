package types

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
	AbiName           AbiField = "name"
	AbiNEvents        AbiField = "nEvents"
	AbiNFunctions     AbiField = "nFunctions"
)

// IsValidAbiField returns true if the given field is a valid sortable Abi field.
func IsValidAbiField(field string) bool {
	switch field {
	case "address", "fileSize", "hasConstructor", "hasFallback", "isEmpty", "isKnown", "lastModDate", "name", "nEvents", "nFunctions":
		return true
	}
	return false
}

// AbiBy returns a comparison function for sorting Abi instances by the given field.
// These comparison functions may be strung together by the CmdAbis function.
func AbiBy(field AbiField, order SortOrder) func(p1, p2 Abi) bool {
	switch field {
	case AbiAddress:
		return func(p1, p2 Abi) bool {
			cmp := p1.Address.Cmp(p2.Address.Address)
			if order == Ascending {
				return cmp == -1
			}
			return cmp == 1
		}
	case AbiFileSize:
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return p1.FileSize < p2.FileSize
			}
			return p1.FileSize > p2.FileSize
		}
	case AbiHasConstructor:
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return !p1.HasConstructor && p2.HasConstructor // False < True
			}
			return p1.HasConstructor && !p2.HasConstructor // True < False
		}
	case AbiHasFallback:
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return !p1.HasFallback && p2.HasFallback // False < True
			}
			return p1.HasFallback && !p2.HasFallback // True < False
		}
	case AbiIsEmpty:
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return !p1.IsEmpty && p2.IsEmpty // False < True
			}
			return p1.IsEmpty && !p2.IsEmpty // True < False
		}
	case AbiIsKnown:
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return !p1.IsKnown && p2.IsKnown // False < True
			}
			return p1.IsKnown && !p2.IsKnown // True < False
		}
	case AbiLastModDate:
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return p1.LastModDate < p2.LastModDate
			}
			return p1.LastModDate > p2.LastModDate
		}
	case AbiName:
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return p1.Name < p2.Name
			}
			return p1.Name > p2.Name
		}
	case AbiNEvents:
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return p1.NEvents < p2.NEvents
			}
			return p1.NEvents > p2.NEvents
		}
	case AbiNFunctions:
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return p1.NFunctions < p2.NFunctions
			}
			return p1.NFunctions > p2.NFunctions
		}
	}
	panic("Should not happen in AbiBy")
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

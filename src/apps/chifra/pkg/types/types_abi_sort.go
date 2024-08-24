package types

type AbiField string

const (
	AbiAddress     AbiField = "address"
	AbiFileSize    AbiField = "fileSize"
	AbiNFunctions  AbiField = "nFunctions"
	AbiNEvents     AbiField = "nEvents"
	AbiIsKnown     AbiField = "isKnown"
	AbiIsEmpty     AbiField = "isEmpty"
	AbiLastModDate AbiField = "lastModDate"
	AbiName        AbiField = "name"
)

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
	case AbiNFunctions:
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return p1.NFunctions < p2.NFunctions
			}
			return p1.NFunctions > p2.NFunctions
		}
	case AbiNEvents:
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return p1.NEvents < p2.NEvents
			}
			return p1.NEvents > p2.NEvents
		}
	case AbiIsKnown:
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return !p1.IsKnown && p2.IsKnown // False < True
			}
			return p1.IsKnown && !p2.IsKnown // True < False
		}
	case AbiIsEmpty:
		return func(p1, p2 Abi) bool {
			if order == Ascending {
				return !p1.IsEmpty && p2.IsEmpty // False < True
			}
			return p1.IsEmpty && !p2.IsEmpty // True < False
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
	}
	panic("Should not happen in AbiBy")
}

// Cmp function that accepts a slice and variadic comparison functions
func Cmp(slice []Abi, orders ...func(p1, p2 Abi) bool) func(i, j int) bool {
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

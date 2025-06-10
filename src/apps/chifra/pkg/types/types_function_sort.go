package types

type FunctionField string

// Fields in the Function struct available for sorting.
const (
	FunctionEncoding        FunctionField = "encoding"
	FunctionName            FunctionField = "name"
	FunctionSignature       FunctionField = "signature"
	FunctionStateMutability FunctionField = "stateMutability"
	FunctionType            FunctionField = "type"
)

// GetSortFieldsFunction returns a []string of valid sort fields for the type.
func GetSortFieldsFunction() []string {
	return []string{"encoding", "name", "signature", "stateMutability", "type"}
}

// FunctionBy returns a comparison function for sorting Function instances by the given field.
// These comparison functions may be strung together by the CmdFunctions function.
func FunctionBy(field FunctionField, order SortOrder) func(p1, p2 Function) bool {
	switch field {
	case FunctionEncoding: // string
		return func(p1, p2 Function) bool {
			if order == Ascending {
				return p1.Encoding < p2.Encoding
			}
			return p1.Encoding > p2.Encoding
		}
	case FunctionName: // string
		return func(p1, p2 Function) bool {
			if order == Ascending {
				return p1.Name < p2.Name
			}
			return p1.Name > p2.Name
		}
	case FunctionSignature: // string
		return func(p1, p2 Function) bool {
			if order == Ascending {
				return p1.Signature < p2.Signature
			}
			return p1.Signature > p2.Signature
		}
	case FunctionStateMutability: // string
		return func(p1, p2 Function) bool {
			if order == Ascending {
				return p1.StateMutability < p2.StateMutability
			}
			return p1.StateMutability > p2.StateMutability
		}
	case FunctionType: // string
		return func(p1, p2 Function) bool {
			if order == Ascending {
				return p1.FunctionType < p2.FunctionType
			}
			return p1.FunctionType > p2.FunctionType
		}

	}
	panic("Should not happen in FunctionBy")
}

// FunctionCmp accepts a slice and variadic comparison functions and returns a functions
// that can be used to sort the slice.
func FunctionCmp(slice []Function, orders ...func(p1, p2 Function) bool) func(i, j int) bool {
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

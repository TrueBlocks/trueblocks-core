package types

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"

type NameField string

// Fields in the Name struct available for sorting.
const (
	NameAddress    NameField = "address"
	NameDecimals   NameField = "decimals"
	NameDeleted    NameField = "deleted"
	NameIsContract NameField = "isContract"
	NameIsCustom   NameField = "isCustom"
	NameIsErc20    NameField = "isErc20"
	NameIsErc721   NameField = "isErc721"
	NameIsPrefund  NameField = "isPrefund"
	NameName       NameField = "name"
	NameSource     NameField = "source"
	NameSymbol     NameField = "symbol"
	NameTags       NameField = "tags"
)

// GetSortFieldsName returns a []string of valid sort fields for the type.
func GetSortFieldsName() []string {
	return []string{"address", "decimals", "deleted", "isContract", "isCustom", "isErc20", "isErc721", "isPrefund", "name", "source", "symbol", "tags"}
}

// NameBy returns a comparison function for sorting Name instances by the given field.
// These comparison functions may be strung together by the CmdNames function.
func NameBy(field NameField, order SortOrder) func(p1, p2 Name) bool {
	switch field {
	case NameAddress: // address
		return func(p1, p2 Name) bool {
			cmp := p1.Address.Cmp(p2.Address.Address)
			if order == Ascending {
				return cmp == -1
			}
			return cmp == 1
		}
	case NameDecimals: // uint64
		return func(p1, p2 Name) bool {
			if order == Ascending {
				return p1.Decimals < p2.Decimals
			}
			return p1.Decimals > p2.Decimals
		}
	case NameDeleted: // bool
		return func(p1, p2 Name) bool {
			if order == Ascending {
				return !p1.Deleted && p2.Deleted
			}
			return p1.Deleted && !p2.Deleted
		}
	case NameIsContract: // bool
		return func(p1, p2 Name) bool {
			if order == Ascending {
				return !p1.IsContract && p2.IsContract
			}
			return p1.IsContract && !p2.IsContract
		}
	case NameIsCustom: // bool
		return func(p1, p2 Name) bool {
			if order == Ascending {
				return !p1.IsCustom && p2.IsCustom
			}
			return p1.IsCustom && !p2.IsCustom
		}
	case NameIsErc20: // bool
		return func(p1, p2 Name) bool {
			if order == Ascending {
				return !p1.IsErc20 && p2.IsErc20
			}
			return p1.IsErc20 && !p2.IsErc20
		}
	case NameIsErc721: // bool
		return func(p1, p2 Name) bool {
			if order == Ascending {
				return !p1.IsErc721 && p2.IsErc721
			}
			return p1.IsErc721 && !p2.IsErc721
		}
	case NameIsPrefund: // bool
		return func(p1, p2 Name) bool {
			if order == Ascending {
				return !p1.IsPrefund && p2.IsPrefund
			}
			return p1.IsPrefund && !p2.IsPrefund
		}
	case NameName: // string
		return func(p1, p2 Name) bool {
			if order == Ascending {
				return p1.Name < p2.Name
			}
			return p1.Name > p2.Name
		}
	case NameSource: // string
		return func(p1, p2 Name) bool {
			if order == Ascending {
				return p1.Source < p2.Source
			}
			return p1.Source > p2.Source
		}
	case NameSymbol: // string
		return func(p1, p2 Name) bool {
			if order == Ascending {
				return p1.Symbol < p2.Symbol
			}
			return p1.Symbol > p2.Symbol
		}
	case NameTags: // string
		return func(p1, p2 Name) bool {
			if order == Ascending {
				return p1.Tags < p2.Tags
			}
			return p1.Tags > p2.Tags
		}

	}
	logger.ShouldNotHappen("in NameBy")
	return nil
}

// NameCmp accepts a slice and variadic comparison functions and returns a functions
// that can be used to sort the slice.
func NameCmp(slice []Name, orders ...func(p1, p2 Name) bool) func(i, j int) bool {
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

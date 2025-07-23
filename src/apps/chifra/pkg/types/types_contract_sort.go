package types

type ContractField string

// Fields in the Contract struct available for sorting.
const (
	ContractAddress     ContractField = "address"
	ContractDate        ContractField = "date"
	ContractLastUpdated ContractField = "lastUpdated"
	ContractName        ContractField = "name"
)

// GetSortFieldsContract returns a []string of valid sort fields for the type.
func GetSortFieldsContract() []string {
	return []string{"address", "date", "lastUpdated", "name"}
}

// ContractBy returns a comparison function for sorting Contract instances by the given field.
// These comparison functions may be strung together by the CmdContracts function.
func ContractBy(field ContractField, order SortOrder) func(p1, p2 Contract) bool {
	switch field {
	case ContractAddress: // address
		return func(p1, p2 Contract) bool {
			cmp := p1.Address.Cmp(p2.Address.Address)
			if order == Ascending {
				return cmp == -1
			}
			return cmp == 1
		}
	case ContractDate: // datetime
		return func(p1, p2 Contract) bool {
			if order == Ascending {
				return p1.Date < p2.Date
			}
			return p1.Date > p2.Date
		}
	case ContractLastUpdated: // timestamp
		return func(p1, p2 Contract) bool {
			if order == Ascending {
				return p1.LastUpdated < p2.LastUpdated
			}
			return p1.LastUpdated > p2.LastUpdated
		}
	case ContractName: // string
		return func(p1, p2 Contract) bool {
			if order == Ascending {
				return p1.Name < p2.Name
			}
			return p1.Name > p2.Name
		}

	}
	panic("Should not happen in ContractBy")
}

// ContractCmp accepts a slice and variadic comparison functions and returns a functions
// that can be used to sort the slice.
func ContractCmp(slice []Contract, orders ...func(p1, p2 Contract) bool) func(i, j int) bool {
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

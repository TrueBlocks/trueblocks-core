// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"

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
			if order == Ascending {
				return p1.Address.LessThan(p2.Address)
			}
			return p2.Address.LessThan(p1.Address)
		}
	case ContractDate: // datetime
		return func(p1, p2 Contract) bool {
			if order == Ascending {
				return p1.Date() < p2.Date()
			}
			return p1.Date() > p2.Date()
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
	logger.ShouldNotHappen("in ContractBy")
	return nil
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

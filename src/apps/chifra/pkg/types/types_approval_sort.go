// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"

type ApprovalField string

// Fields in the Approval struct available for sorting.
const (
	ApprovalAllowance    ApprovalField = "allowance"
	ApprovalDate         ApprovalField = "date"
	ApprovalLastAppBlock ApprovalField = "lastAppBlock"
	ApprovalOwner        ApprovalField = "owner"
	ApprovalSpender      ApprovalField = "spender"
	ApprovalTimestamp    ApprovalField = "timestamp"
	ApprovalToken        ApprovalField = "token"
)

// GetSortFieldsApproval returns a []string of valid sort fields for the type.
func GetSortFieldsApproval() []string {
	return []string{"allowance", "date", "lastAppBlock", "owner", "spender", "timestamp", "token"}
}

// ApprovalBy returns a comparison function for sorting Approval instances by the given field.
// These comparison functions may be strung together by the CmdApprovals function.
func ApprovalBy(field ApprovalField, order SortOrder) func(p1, p2 Approval) bool {
	switch field {
	case ApprovalAllowance: // wei
		return func(p1, p2 Approval) bool {
			if order == Ascending {
				return p1.Allowance.LessThan(&p2.Allowance)
			}
			return p2.Allowance.LessThan(&p1.Allowance)
		}
	case ApprovalDate: // datetime
		return func(p1, p2 Approval) bool {
			if order == Ascending {
				return p1.Date() < p2.Date()
			}
			return p1.Date() > p2.Date()
		}
	case ApprovalLastAppBlock: // blknum
		return func(p1, p2 Approval) bool {
			if order == Ascending {
				return p1.LastAppBlock < p2.LastAppBlock
			}
			return p1.LastAppBlock > p2.LastAppBlock
		}
	case ApprovalOwner: // address
		return func(p1, p2 Approval) bool {
			if order == Ascending {
				return p1.Owner.LessThan(p2.Owner)
			}
			return p2.Owner.LessThan(p1.Owner)
		}
	case ApprovalSpender: // address
		return func(p1, p2 Approval) bool {
			if order == Ascending {
				return p1.Spender.LessThan(p2.Spender)
			}
			return p2.Spender.LessThan(p1.Spender)
		}
	case ApprovalTimestamp: // timestamp
		return func(p1, p2 Approval) bool {
			if order == Ascending {
				return p1.Timestamp < p2.Timestamp
			}
			return p1.Timestamp > p2.Timestamp
		}
	case ApprovalToken: // address
		return func(p1, p2 Approval) bool {
			if order == Ascending {
				return p1.Token.LessThan(p2.Token)
			}
			return p2.Token.LessThan(p1.Token)
		}

	}
	logger.ShouldNotHappen("in ApprovalBy")
	return nil
}

// ApprovalCmp accepts a slice and variadic comparison functions and returns a functions
// that can be used to sort the slice.
func ApprovalCmp(slice []Approval, orders ...func(p1, p2 Approval) bool) func(i, j int) bool {
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

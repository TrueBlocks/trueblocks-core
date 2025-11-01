// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"

type MonitorField string

// Fields in the Monitor struct available for sorting.
const (
	MonitorAddress     MonitorField = "address"
	MonitorDeleted     MonitorField = "deleted"
	MonitorFileSize    MonitorField = "fileSize"
	MonitorIsEmpty     MonitorField = "isEmpty"
	MonitorIsStaged    MonitorField = "isStaged"
	MonitorLastScanned MonitorField = "lastScanned"
	MonitorNRecords    MonitorField = "nRecords"
)

// GetSortFieldsMonitor returns a []string of valid sort fields for the type.
func GetSortFieldsMonitor() []string {
	return []string{"address", "deleted", "fileSize", "isEmpty", "isStaged", "lastScanned", "nRecords"}
}

// MonitorBy returns a comparison function for sorting Monitor instances by the given field.
// These comparison functions may be strung together by the CmdMonitors function.
func MonitorBy(field MonitorField, order SortOrder) func(p1, p2 Monitor) bool {
	switch field {
	case MonitorAddress: // address
		return func(p1, p2 Monitor) bool {
			if order == Ascending {
				return p1.Address.LessThan(p2.Address)
			}
			return p2.Address.LessThan(p1.Address)
		}
	case MonitorDeleted: // bool
		return func(p1, p2 Monitor) bool {
			if order == Ascending {
				return !p1.Deleted && p2.Deleted
			}
			return p1.Deleted && !p2.Deleted
		}
	case MonitorFileSize: // int64
		return func(p1, p2 Monitor) bool {
			if order == Ascending {
				return p1.FileSize < p2.FileSize
			}
			return p1.FileSize > p2.FileSize
		}
	case MonitorIsEmpty: // bool
		return func(p1, p2 Monitor) bool {
			if order == Ascending {
				return !p1.IsEmpty && p2.IsEmpty
			}
			return p1.IsEmpty && !p2.IsEmpty
		}
	case MonitorIsStaged: // bool
		return func(p1, p2 Monitor) bool {
			if order == Ascending {
				return !p1.IsStaged && p2.IsStaged
			}
			return p1.IsStaged && !p2.IsStaged
		}
	case MonitorLastScanned: // uint32
		return func(p1, p2 Monitor) bool {
			if order == Ascending {
				return p1.LastScanned < p2.LastScanned
			}
			return p1.LastScanned > p2.LastScanned
		}
	case MonitorNRecords: // int64
		return func(p1, p2 Monitor) bool {
			if order == Ascending {
				return p1.NRecords < p2.NRecords
			}
			return p1.NRecords > p2.NRecords
		}

	}
	logger.ShouldNotHappen("in MonitorBy")
	return nil
}

// MonitorCmp accepts a slice and variadic comparison functions and returns a functions
// that can be used to sort the slice.
func MonitorCmp(slice []Monitor, orders ...func(p1, p2 Monitor) bool) func(i, j int) bool {
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

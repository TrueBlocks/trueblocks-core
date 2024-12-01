package types

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

// IsValidMonitorField returns true if the given field is a valid sortable Monitor field.
func IsValidMonitorField(field string) bool {
	switch field {
	case "address", "deleted", "fileSize", "isEmpty", "isStaged", "lastScanned", "nRecords":
		return true
	}
	return false
}

// MonitorBy returns a comparison function for sorting Monitor instances by the given field.
// These comparison functions may be strung together by the CmdMonitors function.
func MonitorBy(field MonitorField, order SortOrder) func(p1, p2 Monitor) bool {
	switch field {
	case MonitorAddress: // address
		return func(p1, p2 Monitor) bool {
			cmp := p1.Address.Cmp(p2.Address.Address)
			if order == Ascending {
				return cmp == -1
			}
			return cmp == 1
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
	panic("Should not happen in MonitorBy")
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

package types

type ChunkRecordField string

// Fields in the ChunkRecord struct available for sorting.
const (
	ChunkRecordBloomSize  ChunkRecordField = "bloomSize"
	ChunkRecordIndexSize  ChunkRecordField = "indexSize"
	ChunkRecordRange      ChunkRecordField = "range"
	ChunkRecordRangeDates ChunkRecordField = "rangeDates"
)

// IsValidChunkRecordField returns true if the given field is a valid sortable ChunkRecord field.
func IsValidChunkRecordField(field string) bool {
	switch field {
	case "bloomSize", "indexSize", "range", "rangeDates":
		return true
	}
	return false
}

// ChunkRecordBy returns a comparison function for sorting ChunkRecord instances by the given field.
// These comparison functions may be strung together by the CmdChunkRecords function.
func ChunkRecordBy(field ChunkRecordField, order SortOrder) func(p1, p2 ChunkRecord) bool {
	switch field {
	case ChunkRecordBloomSize: // int64
		return func(p1, p2 ChunkRecord) bool {
			if order == Ascending {
				return p1.BloomSize < p2.BloomSize
			}
			return p1.BloomSize > p2.BloomSize
		}
	case ChunkRecordIndexSize: // int64
		return func(p1, p2 ChunkRecord) bool {
			if order == Ascending {
				return p1.IndexSize < p2.IndexSize
			}
			return p1.IndexSize > p2.IndexSize
		}
	case ChunkRecordRange: // blkrange
		return func(p1, p2 ChunkRecord) bool {
			if order == Ascending {
				return p1.Range < p2.Range
			}
			return p1.Range > p2.Range
		}
	case ChunkRecordRangeDates: // RangeDates
		return func(p1, p2 ChunkRecord) bool {
			cmp := p1.RangeDates.Cmp(p2.RangeDates)
			if order == Ascending {
				return cmp == -1
			}
			return cmp == 1
		}

	}
	panic("Should not happen in ChunkRecordBy")
}

// ChunkRecordCmp accepts a slice and variadic comparison functions and returns a functions
// that can be used to sort the slice.
func ChunkRecordCmp(slice []ChunkRecord, orders ...func(p1, p2 ChunkRecord) bool) func(i, j int) bool {
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

package types

import (
	"reflect"
	"sort"
	"testing"
)

// TestAsSliceOfMaps tests the AsSliceOfMaps function using a table-driven approach.
func TestAsSliceOfMaps(t *testing.T) {
	// Define test cases.
	tests := []struct {
		name       string
		apps       []Appearance
		reversed   bool
		appMapSize int // If non-zero, override the global AppMapSize.
		wantCount  int // Expected total count (should equal len(apps)).
	}{
		{
			name:      "Empty slice",
			apps:      []Appearance{},
			reversed:  false,
			wantCount: 0,
		},
		{
			name: "Less than AppMapSize (ascending sort)",
			apps: []Appearance{
				{BlockNumber: 2, TransactionIndex: 1},
				{BlockNumber: 1, TransactionIndex: 2},
				{BlockNumber: 1, TransactionIndex: 1},
			},
			reversed:   false,
			appMapSize: 20, // using the default value
			wantCount:  3,
		},
		{
			name: "Exactly AppMapSize (ascending sort)",
			apps: []Appearance{
				{BlockNumber: 3, TransactionIndex: 0},
				{BlockNumber: 1, TransactionIndex: 0},
				{BlockNumber: 2, TransactionIndex: 0},
			},
			reversed:   false,
			appMapSize: 3, // override to force one full map exactly
			wantCount:  3,
		},
		{
			name: "More than AppMapSize (ascending sort)",
			apps: []Appearance{
				{BlockNumber: 5, TransactionIndex: 0},
				{BlockNumber: 3, TransactionIndex: 0},
				{BlockNumber: 4, TransactionIndex: 0},
				{BlockNumber: 1, TransactionIndex: 0},
				{BlockNumber: 2, TransactionIndex: 0},
			},
			reversed:   false,
			appMapSize: 2, // force grouping into chunks of 2
			wantCount:  5,
		},
		{
			name: "Less than AppMapSize (reversed sort)",
			apps: []Appearance{
				{BlockNumber: 2, TransactionIndex: 1},
				{BlockNumber: 1, TransactionIndex: 2},
				{BlockNumber: 1, TransactionIndex: 1},
			},
			reversed:   true,
			appMapSize: 20,
			wantCount:  3,
		},
	}

	// Loop over test cases.
	for _, tt := range tests {
		// If the test case specifies an appMapSize override, set the global.
		if tt.appMapSize != 0 {
			AppMapSize = tt.appMapSize
		} else {
			// Reset to default if needed.
			AppMapSize = 20
		}

		// Make a copy of tt.apps because AsSliceOfMaps sorts the slice in place.
		appsCopy := make([]Appearance, len(tt.apps))
		copy(appsCopy, tt.apps)

		// Call the function with T = bool.
		gotMaps, gotCount, err := AsSliceOfMaps[bool](appsCopy, tt.reversed)
		if err != nil {
			t.Fatalf("%q: unexpected error: %v", tt.name, err)
		}
		if gotCount != tt.wantCount {
			t.Errorf("%q: got count %d, want %d", tt.name, gotCount, tt.wantCount)
		}

		// To verify the grouping, we first sort the original slice (using the same logic as in AsSliceOfMaps)
		// to compute the expected order.
		expected := make([]Appearance, len(tt.apps))
		copy(expected, tt.apps)
		if tt.reversed {
			sort.Slice(expected, func(i, j int) bool {
				// For reversed sort, the function swaps i and j (effectively descending).
				if expected[i].BlockNumber == expected[j].BlockNumber {
					return expected[i].TransactionIndex > expected[j].TransactionIndex
				}
				return expected[i].BlockNumber > expected[j].BlockNumber
			})
		} else {
			sort.Slice(expected, func(i, j int) bool {
				if expected[i].BlockNumber == expected[j].BlockNumber {
					return expected[i].TransactionIndex < expected[j].TransactionIndex
				}
				return expected[i].BlockNumber < expected[j].BlockNumber
			})
		}

		// Now, group the expected slice into chunks of size AppMapSize.
		var expectedGroups [][]Appearance
		for i := 0; i < len(expected); i += AppMapSize {
			end := i + AppMapSize
			if end > len(expected) {
				end = len(expected)
			}
			// Make a copy of the chunk.
			group := make([]Appearance, end-i)
			copy(group, expected[i:end])
			expectedGroups = append(expectedGroups, group)
		}

		// Check that the number of maps equals the number of expected groups.
		if len(gotMaps) != len(expectedGroups) {
			t.Errorf("%q: got %d groups, want %d groups", tt.name, len(gotMaps), len(expectedGroups))
		}

		// For each returned map, extract its keys, sort them using the same comparator, and compare with the expected chunk.
		for i, m := range gotMaps {
			var keys []Appearance
			for k := range m {
				keys = append(keys, k)
			}
			if tt.reversed {
				sort.Slice(keys, func(i, j int) bool {
					if keys[i].BlockNumber == keys[j].BlockNumber {
						return keys[i].TransactionIndex > keys[j].TransactionIndex
					}
					return keys[i].BlockNumber > keys[j].BlockNumber
				})
			} else {
				sort.Slice(keys, func(i, j int) bool {
					if keys[i].BlockNumber == keys[j].BlockNumber {
						return keys[i].TransactionIndex < keys[j].TransactionIndex
					}
					return keys[i].BlockNumber < keys[j].BlockNumber
				})
			}

			// If there are fewer groups than expected (because of a test failure), skip the check.
			if i >= len(expectedGroups) {
				continue
			}
			if !reflect.DeepEqual(keys, expectedGroups[i]) {
				t.Errorf("%q: group %d mismatch: got %+v, want %+v", tt.name, i, keys, expectedGroups[i])
			}
		}
	}
}

package config

import (
	"os"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestFilenameFromRange(t *testing.T) {
	type TestType struct {
		name      string
		fileRange base.FileRange
		rangeStr  string
	}
	tests := []TestType{
		{
			name:      "Pads numbers",
			fileRange: base.FileRange{First: 0, Last: 1},
			rangeStr:  "000000000-000000001",
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got, err := base.RangeFromFilenameE(tt.rangeStr); err != nil {
				t.Errorf("FilenameFromRange() error: %v", err)
			} else if got != tt.fileRange {
				t.Errorf("FilenameFromRange() = %v, want %v", got, tt.rangeStr)
			}
			if got := RangeFromRangeString(tt.rangeStr); got != tt.fileRange {
				t.Errorf("FilenameFromRange() = %v, want %v", got, tt.rangeStr)
			}
		})
	}

	fR := base.FileRange{First: 0, Last: 100}
	want := "mainnet/finalized/000000000-000000100.bin"
	got := RangeToFilename("mainnet", &fR)
	parts := strings.Split(got, "unchained"+string(os.PathSeparator))
	if len(parts) != 2 || parts[1] != want {
		t.Errorf("FilenameFromRange() = %v, want %v", got, parts[1])
	}
}

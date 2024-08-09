// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package base

import (
	"fmt"
	"os"
	"strings"
	"testing"
)

func TestFileRange(t *testing.T) {
	type TestType struct {
		fileName string
		want     FileRange
		errored  bool
	}
	tests := []TestType{
		{
			fileName: "0.bin",
			want:     FileRange{0, 0},
		},
		{
			fileName: "illformed-x",
			want:     FileRange{0, 0},
			errored:  true,
		},
		{
			fileName: "9991-9909.bin",
			want:     FileRange{9991, 9909},
		},
		{
			fileName: "000100001-000100002.bin",
			want:     FileRange{100001, 100002},
		},
		{
			fileName: "891919121.bin",
			want:     FileRange{0, 891919121},
		},
		{
			fileName: "/unchained/mainnet/blooms/013433393-013436307.bloom",
			want:     FileRange{13433393, 13436307},
		},
	}

	check := func(t *testing.T, tt TestType, s FileRange, e error) {
		if tt.errored && e == nil {
			t.Error("Expected an error but got none", s)
		} else if !tt.errored && e != nil {
			t.Error("Unexpected error", e)
		}
	}

	for _, tt := range tests {
		s, err := RangeFromFilenameE(tt.fileName)
		check(t, tt, s, err)
		fmt.Println(s)
	}
}

func TestFilenameFromRange(t *testing.T) {
	type TestType struct {
		name      string
		fileRange FileRange
		rangeStr  string
	}
	tests := []TestType{
		{
			name:      "Pads numbers",
			fileRange: FileRange{0, 1},
			rangeStr:  "000000000-000000001",
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got, err := RangeFromFilenameE(tt.rangeStr); err != nil {
				t.Errorf("FilenameFromRange() error: %v", err)
			} else if got != tt.fileRange {
				t.Errorf("FilenameFromRange() = %v, want %v", got, tt.rangeStr)
			}
			if got := RangeFromRangeString(tt.rangeStr); got != tt.fileRange {
				t.Errorf("FilenameFromRange() = %v, want %v", got, tt.rangeStr)
			}
		})
	}

	fR := FileRange{0, 100}
	want := "mainnet/finalized/000000000-000000100.bin"
	got := fR.RangeToFilename("mainnet")
	parts := strings.Split(got, "unchained"+string(os.PathSeparator))
	if len(parts) != 2 || parts[1] != want {
		t.Errorf("FilenameFromRange() = %v, want %v", got, parts[1])
	}
}

func Test_RangeRangeIntersect(t *testing.T) {
	type TestType struct {
		name  string
		left  FileRange
		right FileRange
		want  []bool
	}
	tests := []TestType{
		{
			name:  "right less than left",
			left:  FileRange{200, 300},
			right: FileRange{100, 150},
			want:  []bool{false, false, false, true, false},
		},
		{
			name:  "fully overlap",
			left:  FileRange{200, 300},
			right: FileRange{100, 400},
			want:  []bool{true, false, false, true, true},
		},
		{
			name:  "fully contained",
			left:  FileRange{200, 300},
			right: FileRange{250, 275},
			want:  []bool{true, true, true, false, false},
		},
		{
			name:  "lefts align",
			left:  FileRange{200, 300},
			right: FileRange{200, 400},
			want:  []bool{true, true, false, false, true},
		},
		{
			name:  "left greater than",
			left:  FileRange{200, 300},
			right: FileRange{250, 400},
			want:  []bool{true, true, false, false, true},
		},
		{
			name:  "left aligns right",
			left:  FileRange{200, 300},
			right: FileRange{300, 400},
			want:  []bool{true, true, false, false, true},
		},
		{
			name:  "rights align",
			left:  FileRange{200, 300},
			right: FileRange{250, 300},
			want:  []bool{true, true, true, false, false},
		},
		{
			name:  "left greater than right",
			left:  FileRange{200, 300},
			right: FileRange{350, 400},
			want:  []bool{false, false, false, false, true},
		},
	}

	for _, tt := range tests {
		s := tt.left.Intersects(tt.right)
		if s != tt.want[0] {
			t.Error("Test", tt.name, "failed Intersects.")
		}
		s = tt.left.IntersectsB(tt.right.First)
		if s != tt.want[1] {
			t.Error("Test", tt.name, "failed IntersectsB 1.")
		}
		s = tt.left.IntersectsB(tt.right.Last)
		if s != tt.want[2] {
			t.Error("Test", tt.name, "failed IntersectsB 2.")
		}
		// s = tt.left.LaterThan(tt.right)
		// if s != tt.want[3] {
		// 	t.Error("Test", tt.name, "failed LaterThan.")
		// }
		s = tt.left.LaterThanB(tt.right.First)
		if s != tt.want[3] {
			t.Error("Test", tt.name, "failed LaterThanB.")
		}
		// s = tt.left.EarlierThan(tt.right)
		// if s != tt.want[4] {
		// 	t.Error("Test", tt.name, "failed EarlierThan.")
		// }
		s = tt.left.EarlierThanB(tt.right.Last)
		if s != tt.want[4] {
			t.Error("Test", tt.name, "failed EarlierThanB.")
		}
	}
}

func Test_Relative(t *testing.T) {
	type TestType struct {
		name  string
		left  FileRange
		right FileRange
		want  []bool
	}
	tests := []TestType{
		{
			name:  "left preceeds right",
			left:  FileRange{100, 200},
			right: FileRange{300, 400},
			want:  []bool{true, false},
		},
		{
			name:  "left preceeds right",
			left:  FileRange{300, 400},
			right: FileRange{100, 200},
			want:  []bool{false, true},
		},
	}
	for _, tt := range tests {
		s := tt.left.Preceeds(tt.right, false)
		if s != tt.want[0] {
			t.Error("Test", tt.name, "failed Preceeds.")
		}
		s = tt.left.Follows(tt.right, false)
		if s != tt.want[1] {
			t.Error("Test", tt.name, "failed Follows.")
		}
	}
}

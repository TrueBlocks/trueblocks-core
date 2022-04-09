// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cache

import (
	"fmt"
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
		s, err := RangeFromFilename(tt.fileName)
		check(t, tt, s, err)
		fmt.Println(s)
	}
}

func TestFilenameFromRange(t *testing.T) {
	type args struct {
		fileRange FileRange
		extension string
	}
	tests := []struct {
		name string
		args args
		want string
	}{
		{
			name: "Pads numbers",
			args: args{
				fileRange: FileRange{
					First: 0,
					Last:  1,
				},
				extension: "bloom",
			},
			want: "000000000-000000001.bloom",
		},
		{
			name: "Random range 1",
			args: args{
				fileRange: FileRange{
					First: 14040187,
					Last:  14043115,
				},
				extension: "bloom",
			},
			want: "014040187-014043115.bloom",
		},
		{
			name: "Random range 2",
			args: args{
				fileRange: FileRange{
					First: 1371353,
					Last:  1504328,
				},
				extension: "bloom",
			},
			want: "001371353-001504328.bloom",
		},
		{
			name: "Works without extension",
			args: args{
				fileRange: FileRange{
					First: 1371353,
					Last:  1504328,
				},
				extension: "",
			},
			want: "001371353-001504328",
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := FilenameFromRange(tt.args.fileRange, tt.args.extension); got != tt.want {
				t.Errorf("FilenameFromRange() = %v, want %v", got, tt.want)
			}
		})
	}
}

func Test_RangeRangeIntersect(t *testing.T) {
	type TestType struct {
		name string
		r1   FileRange
		r2   FileRange
		want []bool
	}
	tests := []TestType{
		{
			name: "right less than left",
			r1:   FileRange{200, 300},
			r2:   FileRange{100, 150},
			want: []bool{false, false, false},
		},
		{
			name: "fully overlap",
			r1:   FileRange{200, 300},
			r2:   FileRange{100, 400},
			want: []bool{true, false, false},
		},
		{
			name: "fully contained",
			r1:   FileRange{200, 300},
			r2:   FileRange{250, 275},
			want: []bool{true, true, true},
		},
		{
			name: "lefts align",
			r1:   FileRange{200, 300},
			r2:   FileRange{200, 400},
			want: []bool{true, true, false},
		},
		{
			name: "left greater than",
			r1:   FileRange{200, 300},
			r2:   FileRange{250, 400},
			want: []bool{true, true, false},
		},
		{
			name: "left aligns right",
			r1:   FileRange{200, 300},
			r2:   FileRange{300, 400},
			want: []bool{true, true, false},
		},
		{
			name: "rights align",
			r1:   FileRange{200, 300},
			r2:   FileRange{250, 300},
			want: []bool{true, true, true},
		},
		{
			name: "left greater than right",
			r1:   FileRange{200, 300},
			r2:   FileRange{350, 400},
			want: []bool{false, false, false},
		},
	}

	for _, tt := range tests {
		s := tt.r1.Intersects(tt.r2)
		if s != tt.want[0] {
			t.Error("Test", tt.name, "failed.")
		}
		s = tt.r1.BlockIntersects(tt.r2.First)
		if s != tt.want[1] {
			t.Error("Test", tt.name, "failed.")
		}
		s = tt.r1.BlockIntersects(tt.r2.Last)
		if s != tt.want[2] {
			t.Error("Test", tt.name, "failed.")
		}
	}
}

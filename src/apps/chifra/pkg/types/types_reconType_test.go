package types

import (
	"testing"
)

func TestReconTypeString(t *testing.T) {
	tests := []struct {
		input    PostType
		expected string
	}{
		{Genesis, "genesis-diff"},
		{DiffDiff, "diff-diff"},
		{SameSame, "same-same"},
		{DiffSame, "diff-same"},
		{SameDiff, "same-diff"},
		{Invalid, "invalid"},
		{ShouldNotHappen, "invalid"},
		{DiffDiff, "diff-diff"},
		{SameSame, "same-same"},
		{DiffSame, "diff-same"},
	}

	for _, tt := range tests {
		result := tt.input.String()
		if result != tt.expected {
			t.Errorf("PostType(%d).String() = %q; want %q", tt.input, result, tt.expected)
		}
	}
}

func TestReconTypeCombinationStrings(t *testing.T) {
	tests := []struct {
		input    PostType
		expected string
	}{
		{Genesis, "genesis-diff"},
		{Genesis, "genesis-diff"},
		{DiffDiff, "diff-diff"},
		{SameSame, "same-same"},
		{DiffSame, "diff-same"},
	}

	for _, tt := range tests {
		result := tt.input.String()
		if result != tt.expected {
			t.Errorf("PostType(%d).String() = %q; want %q", tt.input, result, tt.expected)
		}
	}
}

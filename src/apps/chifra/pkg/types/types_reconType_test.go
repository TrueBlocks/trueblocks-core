package types

import (
	"testing"
)

func TestReconTypeString(t *testing.T) {
	tests := []struct {
		input    ReconType
		expected string
	}{
		{Genesis, "genesis-diff"},
		{DiffDiff, "diff-diff"},
		{SameSame, "same-same"},
		{DiffSame, "diff-same"},
		{SameDiff, "same-diff"},
		{Invalid, "invalid"},
		{ShouldNotHappen, "invalid"},
		{First | DiffDiff, "first-diff-diff"},
		{Last | SameSame, "same-last"},
		{First | Last | DiffSame, "first-diff-last"},
	}

	for _, tt := range tests {
		result := tt.input.String()
		if result != tt.expected {
			t.Errorf("ReconType(%d).String() = %q; want %q", tt.input, result, tt.expected)
		}
	}
}

func TestReconTypeBitwiseOperations(t *testing.T) {
	if (DiffDiff & First) != 0 {
		t.Errorf("DiffDiff should not have First flag set")
	}

	if (First|DiffDiff)&First == 0 {
		t.Errorf("First flag should be detected in First | DiffDiff")
	}

	if (First|Last|DiffSame)&Last == 0 {
		t.Errorf("Last flag should be detected in First | Last | DiffSame")
	}
}

func TestReconTypeCombinationStrings(t *testing.T) {
	tests := []struct {
		input    ReconType
		expected string
	}{
		{First | Genesis, "genesis-diff"},
		{First | Last | Genesis, "genesis-diff"},
		{First | Last | DiffDiff, "first-diff-diff-last"},
		{First | Last | SameSame, "first-last"},
		{First | Last | DiffSame, "first-diff-last"},
	}

	for _, tt := range tests {
		result := tt.input.String()
		if result != tt.expected {
			t.Errorf("ReconType(%d).String() = %q; want %q", tt.input, result, tt.expected)
		}
	}
}

package base

import (
	"testing"
)

func TestFormattedValue(t *testing.T) {
	vv := NewWei(0)
	vv, _ = vv.SetString("599853750352891471017612", 10)
	tests := []struct {
		name     string
		bigIn    *Wei
		asEther  bool
		decimals int
		want     string
	}{
		{
			name:     "Test with positive value",
			bigIn:    NewWei(1000000000000000000),
			asEther:  true,
			decimals: 18,
			want:     "1",
		},
		{
			name:     "Test with negative value",
			bigIn:    NewWei(-1000000000000000000),
			asEther:  true,
			decimals: 18,
			want:     "-1",
		},
		{
			name:     "Test with zero value",
			bigIn:    NewWei(0),
			asEther:  true,
			decimals: 18,
			want:     "0",
		},
		{
			name:     "Test with large positive value",
			bigIn:    NewWei(123456789012345678),
			asEther:  true,
			decimals: 18,
			want:     "0.123456789012345678",
		},
		{
			name:     "Test with large negative value",
			bigIn:    NewWei(-123456789012345678),
			asEther:  true,
			decimals: 18,
			want:     "-0.123456789012345678",
		},
		{
			name:     "Test with non-ether value",
			bigIn:    NewWei(1234567890),
			asEther:  false,
			decimals: 0,
			want:     "1234567890",
		},
		{
			name:     "Test with non-ether value and decimals",
			bigIn:    NewWei(1234567890),
			asEther:  false,
			decimals: 2, // decimals is not used when asEther is false
			want:     "1234567890",
		},
		{
			name:     "live test",
			bigIn:    vv,
			asEther:  true,
			decimals: 18,
			want:     "599853.750352891471017612",
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got := tt.bigIn.ToEtherStr(tt.decimals)
			if !tt.asEther {
				got = tt.bigIn.String()
			}
			if got != tt.want {
				t.Errorf("%s TestFv = %v, want %v", tt.name, got, tt.want)
			}
		})
	}
}

func TestToEther(t *testing.T) {
	type testCase struct {
		input    *Wei
		expected string
	}

	v1, _ := new(Wei).SetString("123456789012345678", 10)
	v2, _ := new(Wei).SetString("1234567890123456789", 10)
	var tests = []testCase{
		{NewWei(0), "0"},
		{NewWei(1), "0.000000000000000001"},
		{v1, "0.123456789012345678"},
		{v2, "1.234567890123456789"},
	}

	for _, test := range tests {
		got := ToEther(test.input).Text('f', -18)
		if got != test.expected {
			t.Errorf("ToEther(%v) want %v, got %v", test.input, test.expected, got)
		}
		got = ToEther(test.input).String()
		if got != test.expected {
			t.Errorf("ToEther(%v) want %v, got %v", test.input, test.expected, got)
		}
		got = ToEther(test.input).String()
		if got != test.expected {
			t.Errorf("ToEther(%v) want %v, got %v", test.input, test.expected, got)
		}
	}
}

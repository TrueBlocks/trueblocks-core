package base

import (
	"fmt"
	"testing"
)

func TestFormattedValue(t *testing.T) {
	vv := NewMyWei(0)
	vv, _ = vv.SetString("599853750352891471017612", 10)
	tests := []struct {
		name     string
		bigIn    *MyWei
		asEther  bool
		decimals int
		want     string
	}{
		{
			name:     "Test with positive value",
			bigIn:    NewMyWei(1000000000000000000),
			asEther:  true,
			decimals: 18,
			want:     "1",
		},
		{
			name:     "Test with negative value",
			bigIn:    NewMyWei(-1000000000000000000),
			asEther:  true,
			decimals: 18,
			want:     "-1",
		},
		{
			name:     "Test with zero value",
			bigIn:    NewMyWei(0),
			asEther:  true,
			decimals: 18,
			want:     "0",
		},
		{
			name:     "Test with large positive value",
			bigIn:    NewMyWei(123456789012345678),
			asEther:  true,
			decimals: 18,
			want:     "0.123456789012345678",
		},
		{
			name:     "Test with large negative value",
			bigIn:    NewMyWei(-123456789012345678),
			asEther:  true,
			decimals: 18,
			want:     "-0.123456789012345678",
		},
		{
			name:     "Test with non-ether value",
			bigIn:    NewMyWei(1234567890),
			asEther:  false,
			decimals: 0,
			want:     "1234567890",
		},
		{
			name:     "Test with non-ether value and decimals",
			bigIn:    NewMyWei(1234567890),
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
			got := FormattedValue(tt.bigIn, tt.asEther, tt.decimals)
			if got != tt.want {
				t.Errorf("%s FormattedValue() = %v, want %v", tt.name, got, tt.want)
			}
		})
	}
}

func TestToEther(t *testing.T) {
	type TestCase struct {
		input    *MyWei
		expected string
	}

	v1, _ := new(MyWei).SetString("123456789012345678", 10)
	v2, _ := new(MyWei).SetString("1234567890123456789", 10)
	var tests = []TestCase{
		{NewMyWei(0), "0"},
		{NewMyWei(1), "0.000000000000000001"},
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
		//lint:ignore S1025 sorry
		got = fmt.Sprintf("%s", ToEther(test.input))
		if got != test.expected {
			t.Errorf("ToEther(%v) want %v, got %v", test.input, test.expected, got)
		}
	}
}

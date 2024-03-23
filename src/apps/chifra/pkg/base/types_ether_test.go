package base

import (
	"fmt"
	"math/big"
	"testing"
)

func TestFormatValue(t *testing.T) {
	vv := big.NewInt(0)
	vv, _ = vv.SetString("599853750352891471017612", 10)
	tests := []struct {
		name     string
		in       big.Int
		asEther  bool
		decimals int
		want     string
	}{
		{
			name:     "Test with positive value",
			in:       *big.NewInt(1000000000000000000),
			asEther:  true,
			decimals: 18,
			want:     "1",
		},
		{
			name:     "Test with negative value",
			in:       *big.NewInt(-1000000000000000000),
			asEther:  true,
			decimals: 18,
			want:     "-1",
		},
		{
			name:     "Test with zero value",
			in:       *big.NewInt(0),
			asEther:  true,
			decimals: 18,
			want:     "0",
		},
		{
			name:     "Test with large positive value",
			in:       *big.NewInt(123456789012345678),
			asEther:  true,
			decimals: 18,
			want:     "0.123456789012345678",
		},
		{
			name:     "Test with large negative value",
			in:       *big.NewInt(-123456789012345678),
			asEther:  true,
			decimals: 18,
			want:     "-0.123456789012345678",
		},
		{
			name:     "Test with non-ether value",
			in:       *big.NewInt(1234567890),
			asEther:  false,
			decimals: 0,
			want:     "1234567890",
		},
		{
			name:     "Test with non-ether value and decimals",
			in:       *big.NewInt(1234567890),
			asEther:  false,
			decimals: 2, // decimals is not used when asEther is false
			want:     "1234567890",
		},
		{
			name:     "live test",
			in:       *vv,
			asEther:  true,
			decimals: 18,
			want:     "599853.750352891471017612",
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got := FormattedValue(tt.in, tt.asEther, tt.decimals)
			if got != tt.want {
				t.Errorf("%s FormattedValue() = %v, want %v", tt.name, got, tt.want)
			}
		})
	}
}

func TestWei2EtherStr(t *testing.T) {
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
		got := ToEther2(test.input).Text('f', -18)
		if got != test.expected {
			t.Errorf("ToEther2(%v) want %v, got %v", test.input, test.expected, got)
		}
		got = ToEther2(test.input).String()
		if got != test.expected {
			t.Errorf("ToEther2(%v) want %v, got %v", test.input, test.expected, got)
		}
		//lint:ignore S1025 sorry
		got = fmt.Sprintf("%s", ToEther2(test.input))
		if got != test.expected {
			t.Errorf("ToEther2(%v) want %v, got %v", test.input, test.expected, got)
		}
	}
}

package base

import (
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

package base

import (
	"encoding/json"
	"fmt"
	"math/big"
	"testing"
)

func TestFormattedValue(t *testing.T) {
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

// TestEtherUnmarshal tests the UnmarshalJSON method of Ether. Note this is the whole
// point of the Ether type: to be able to unmarshal JSON strings into big.Floats.
func TestUnmarshalEther(t *testing.T) {
	type withEther struct {
		Name  string  `json:"name"`
		Value *Ether  `json:"value"`
		Float float64 `json:"float"`
	}
	type withBigFloat struct {
		Name  string     `json:"name"`
		Value *big.Float `json:"value"`
		Float float64    `json:"float"`
	}

	type TestCase struct {
		input    string
		expected string
	}

	var tests = []TestCase{
		{
			input:    "1",
			expected: "1",
		},
		{
			input:    `0.000000000000000001`,
			expected: "0.000000000000000001",
		},
		{
			input:    `0.123456789012345678`,
			expected: "0.123456789012345678",
		},
		{
			input:    `1.234567890123456789`,
			expected: "1.234567890123456789",
		},
	}

	quoted := []string{}
	unquoted := []string{}
	for _, test := range tests {
		unquoted = append(unquoted, test.input)
		quoted = append(quoted, fmt.Sprintf("\"%s\"", test.input))
	}

	expected := [][]bool{
		{true, true, true, true, true, true, true, true},     // unquoted both Ether and Float work
		{true, false, true, false, true, false, true, false}, // quoted only Ether works (that's the point)
	}

	for i, inputArray := range [][]string{unquoted, quoted} { /* four quoted and four unquoted */
		for j, inputStr := range inputArray {
			tag := fmt.Sprintf("%s%d", []string{"unquoted", "quoted"}[i], j)
			ethName := fmt.Sprintf("ether-%s", tag)
			bigName := fmt.Sprintf("big-%s", tag)
			passEther := expected[i][j*2]
			passFloat := expected[i][j*2+1]
			// fmt.Printf("Running test %s [%d][%d] (expects: %t, %t)\n", tag, i, j, passEther, passFloat)

			if e, ok := new(Ether).SetString(inputStr); !ok {
				if passEther {
					t.Errorf("error: %s %s %s", tag, ethName, inputStr)
					continue
				}
			} else {
				fe := e.Float64()
				se := withEther{
					Name:  ethName,
					Value: e,
					Float: fe,
				}
				if eJson, err := json.MarshalIndent(&se, "", "  "); err != nil {
					if passEther {
						t.Errorf("error: %s %s %s", tag, ethName, inputStr)
						continue
					}
				} else {
					t.Log(eJson)
					ve := se.Value
					if err := ve.UnmarshalJSON([]byte(inputStr)); err != nil {
						if passEther {
							t.Errorf("error: %s %s %s", tag, ethName, inputStr)
							continue
						}
					}
				}
			}

			if f, ok := new(big.Float).SetString(inputStr); !ok {
				if passFloat {
					t.Errorf("error: %s %s", ethName, inputStr)
					continue
				}

			} else {
				ff, _ := f.Float64()
				sf := withBigFloat{
					Name:  bigName,
					Value: f,
					Float: ff,
				}
				if fJson, err := json.MarshalIndent(&sf, "", "  "); err != nil {
					if passFloat {
						t.Errorf("error: %s %s %s", tag, ethName, inputStr)
						continue
					}
				} else {
					t.Log(fJson)
					ve := sf.Value
					if err := ve.UnmarshalText([]byte(inputStr)); err != nil {
						if passFloat {
							t.Errorf("error: %s %s %s", tag, ethName, inputStr)
							continue
						}
					}
				}
			}

			if !passEther && !passFloat {
				t.Errorf("error: %s %s", ethName, inputStr)
			}
		}
	}
}

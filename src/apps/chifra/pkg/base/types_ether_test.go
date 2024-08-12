package base

import (
	"encoding/json"
	"fmt"
	"math/big"
	"testing"
)

// TestUnmarshalEther tests the UnmarshalJSON method of Ether. Note this is the whole
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

	type testCase struct {
		input    string
		expected string
	}

	var tests = []testCase{
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

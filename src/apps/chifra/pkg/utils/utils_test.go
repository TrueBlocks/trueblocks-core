package utils

// TODO: Re-enable these tests
// func TestWei2EtherStr(t *testing.T) {
// 	type TestCase struct {
// 		input    *big.Int
// 		expected string
// 	}

// 	var tests = []TestCase{
// 		{new(big.Int).SetUint64(0), "0.000000000000000000"},
// 		{new(big.Int).SetUint64(1), "0.000000000000000001"},
// 		{new(big.Int).SetString("12345678901234567890"), "1.2345678901234567890"},
// 		{Wei(12345678901234567890), "12.345678901234567890"},
// 	}

// 	for _, test := range tests {
// 		actual := test.input.EtherStr()
// 		if actual != test.expected {
// 			t.Errorf("Wei2EtherStr(%v) = %v, expected %v", test.input, actual, test.expected)
// 		}
// 	}
// }

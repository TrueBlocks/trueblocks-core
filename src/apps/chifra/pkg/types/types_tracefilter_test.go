package types

import (
	"testing"
)

func TestTraceFilter(t *testing.T) {
	// seen := uint64(0)
	// for traceIndex, trace := range traces {
	// 	fmt.Println(strings.Repeat("-", 80))
	// 	for _, filter := range filters {
	// 		f := TraceFilter{}
	// 		f.ParseBangString(chain, filter.bang)
	// 		// if index > 0 && w > 7 {
	// 		// 	filter.exp = false
	// 		// }
	// 		if traceIndex == 0 && (strings.Contains(filter.name, "after") || strings.Contains(filter.name, "cnt")) {
	// 			filter.exp = false
	// 		}
	// 		passed, reason := f.Passes(&trace, uint64(traceIndex), seen)
	// 		fmt.Println(filter.name, filter.exp, passed, reason, f)
	// 		if passed {
	// 			fmt.Println(colors.Green, "PASSED", f, colors.Off)
	// 		}
	// 		// fmt.Println(m, "to:", trace.Action.To, "from:", trace.Action.From, passed)
	// 		if passed != filter.exp {
	// 			seen++
	// 			t.Errorf("Filter %v failed because %s", filter, reason)
	// 		}
	// 	}
	// }
}

// type Thing struct {
// 	name string
// 	bang string
// 	exp  bool
// }

// var traces = []Trace{
// 	{
// 		BlockNumber: 30,
// 		Action: &TraceAction{
// 			From: base.HexToAddress("0xffffffff00000000000000000000000000000000"),
// 			To:   base.HexToAddress("0x0000000000000000000000000000000011111111"),
// 		},
// 	},
// 	{
// 		BlockNumber: 30,
// 		Action: &TraceAction{
// 			From: base.HexToAddress("0xffffffff00000000000000000000000000000000"),
// 			To:   base.HexToAddress("0x0000000000000000000000000000000011111111"),
// 		},
// 	},
// }

// var filters = []Thing{
// 	{"empty:                ", "", true},
// 	{"too high fromBlock:   ", "31!", false},
// 	{"just right fromBlock: ", "30!", true},
// 	{"too low toBlock:      ", "10!20!", false},
// 	{"just right toBlock:   ", "10!40!", true},
// 	{"not fromAddr:         ", "10!40!0x0000000000000000000000000000000011111111!", false},
// 	{"yes fromAddr:         ", "10!40!0xffffffff00000000000000000000000000000000!", true},
// 	{"no toAddr:            ", "10!40!0xffffffff00000000000000000000000000000000!0xffffffff00000000000000000000000000000000!", false},
// 	{"yes toAddr:           ", "10!40!0xffffffff00000000000000000000000000000000!0x0000000000000000000000000000000011111111!", true},
// 	{"not after:            ", "10!40!0xffffffff00000000000000000000000000000000!0x0000000000000000000000000000000011111111!40!", false},
// 	{"after ok:             ", "10!40!0xffffffff00000000000000000000000000000000!0x0000000000000000000000000000000011111111!1!", true},
// 	{"zero cnt:             ", "10!40!0xffffffff00000000000000000000000000000000!0x0000000000000000000000000000000011111111!1!0!", true},
// 	{"ok cnt:               ", "10!40!0xffffffff00000000000000000000000000000000!0x0000000000000000000000000000000011111111!1!1!", true},
// }

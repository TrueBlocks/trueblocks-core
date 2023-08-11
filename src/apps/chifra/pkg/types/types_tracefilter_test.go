package types

import (
	"fmt"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestTraceFilter(t *testing.T) {
	traces := []SimpleTrace{
		{
			BlockNumber: 10,
			Action: &SimpleTraceAction{
				From: base.HexToAddress("0xffffffff00000000000000000000000000000000"),
				To:   base.HexToAddress("0x0000000000000000000000000000000011111111"),
			},
		},
	}
	for _, trace := range traces {
		trace := trace
		fmt.Println(trace.Action)
	}
	type Thing struct {
		bang string
		exp  bool
	}
	filters := []Thing{
		{"!!!0x0000000000000000000000000000000011111111!!", true},
		{"!!0x0000000000000000000000000000000011111111!!!", false},
		{"!!!0xffffffff00000000000000000000000000000000!!", false},
		{"!!0xffffffff00000000000000000000000000000000!!!", true},
		{"9!!0xffffffff00000000000000000000000000000000!0x0000000000000000000000000000000011111111!!", true},
		{"10!!0xffffffff00000000000000000000000000000000!0x0000000000000000000000000000000011111111!!", true},
		{"11!!0xffffffff00000000000000000000000000000000!0x0000000000000000000000000000000011111111!!", false},
	}

	seen := uint64(0)
	cnt := uint64(0)
	for _, trace := range traces {
		trace := trace
		for _, filter := range filters {
			filter := filter
			f := SimpleTraceFilter{}
			m, _ := f.ParseBangString(filter.bang)
			passed, reason := f.Passes(&trace, seen, cnt)
			if passed != filter.exp {
				t.Errorf("Filter %v failed because %s", filter, reason)
			}
			fmt.Println(m, "to:", trace.Action.To, "from:", trace.Action.From, passed)
			fmt.Println()
			seen++
		}
	}
}

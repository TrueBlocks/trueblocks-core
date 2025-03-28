package pricing

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type priceDebugger struct {
	address     base.Address
	symbol      string
	blockNumber base.Blknum
	source1     base.Address
	theCall1    string
	source2     base.Address
	theCall2    string
	first       base.Address
	second      base.Address
	reversed    bool
	float0      *base.Float
	float1      *base.Float
	float2      *base.Float
	int0        *base.Wei
	int1        *base.Wei
	bigPrice    *base.Float
	price       base.Float
	source      string
}

// TODO: Much of this reporting could be removed by using a String function on the types.Statement
func (r *priceDebugger) report(msg string) {
	debugging := true
	if debugging {
		isStable := strings.Contains(msg, "stable-coin")
		if isStable {
			msg = ""
		}

		logger.TestLog(true, "=========================================================")
		logger.TestLog(true, "===> PRICING FOR", r.address, "("+r.symbol+")", msg)
		logger.TestLog(true, "=========================================================")

		if isStable {
			logger.TestLog(true, "stable-coin")
			return
		}

		logger.TestLog(true, "blockNumber:        ", r.blockNumber)
		logger.TestLog(true, "source1 (uni/mkr):  ", r.source1)
		logger.TestLog(true, "str0 (call):        ", r.theCall1)
		if r.source2 != base.ZeroAddr {
			logger.TestLog(true, "source2 (pair):     ", r.source2)
		}
		logger.TestLog(true, "str1 (call/data):   ", r.theCall2)
		if r.first != base.ZeroAddr {
			logger.TestLog(true, "first:              ", r.first)
		}
		if r.second != base.ZeroAddr {
			logger.TestLog(true, "second:             ", r.second)
			logger.TestLog(true, "reversed:           ", r.reversed)
		}
		if r.int0 != nil {
			logger.TestLog(true, "int0:               ", r.int0)
		}
		if r.int1 != nil {
			logger.TestLog(true, "int1:               ", r.int1)
		}
		if r.float0 != nil {
			logger.TestLog(true, "float0:             ", r.float0.String())
		}
		if r.float1 != nil {
			logger.TestLog(true, "float1:             ", r.float1.String())
		}
		if r.float2 != nil {
			logger.TestLog(true, "float2:             ", r.float2.String())
		}
		logger.TestLog(true, "bigPrice:           ", r.bigPrice.String())
		logger.TestLog(true, "price:              ", r.price.String())
		logger.TestLog(true, "source:             ", r.source)
	}
}

package pricing

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/call"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var (
	makerMedianizer = base.HexToAddress("0x729d19f657bd0614b4985cf1d82531c67569197b")
	makerDeployment = base.Blknum(3684349)
)

func priceUsdMaker(conn *rpc.Connection, statement *types.Statement) (base.Float, string, error) {
	if statement.BlockNumber <= makerDeployment {
		msg := fmt.Sprintf("Block %d is prior to deployment (%d) of Maker. No fallback pricing method", statement.BlockNumber, makerDeployment)
		logger.TestLog(true, msg)
		return *base.ZeroFloat, "eth-not-priced-pre-maker", nil
	}

	msg := fmt.Sprintf("Block %d is prior to deployment (%d) of Uniswap V2. Falling back to Maker (%s)", statement.BlockNumber, uniswapFactoryV2_deployed, makerMedianizer)
	logger.TestLog(true, msg)
	theCall := "peek()"

	contractCall, _, err := call.NewContractCall(conn, makerMedianizer, theCall)
	if err != nil {
		wrapped := fmt.Errorf("the --calldata value provided (%s) was not found: %s", theCall, err)
		return *base.ZeroFloat, "not-priced", wrapped
	}

	contractCall.BlockNumber = statement.BlockNumber
	artFunc := func(str string, function *types.Function) error {
		return articulate.ArticulateFunction(function, "", str[2:])
	}
	result, err := contractCall.Call(artFunc)
	if err != nil {
		return *base.ZeroFloat, "not-priced", err
	}

	divisor := new(base.Wei)
	divisor.SetString("1000000000000000000", 10)

	// TODO: Since Dawid fixed the articulate code, we should use the value at results["val_1"] instead of this
	// TODO: hacky string manipulation
	trimmed := strings.TrimPrefix(string(result.ReturnedBytes), "0x")
	trimmed = trimmed[:64]
	int0 := new(base.Wei)
	int0.SetString(trimmed, 16)
	int0 = new(base.Wei).Mul(int0, new(base.Wei).SetInt64(100000))
	int1 := new(base.Wei).Quo(int0, divisor)

	bigPrice := new(base.Float).SetRawWei(int1)
	bigPrice = new(base.Float).Quo(bigPrice, new(base.Float).SetInt64(100000))
	source := "maker"
	r := priceDebugger{
		address:     statement.Asset,
		symbol:      statement.Symbol,
		blockNumber: statement.BlockNumber,
		source1:     makerMedianizer,
		theCall1:    theCall,
		source2:     base.ZeroAddr,
		theCall2:    "0x" + trimmed,
		first:       base.ZeroAddr,
		second:      base.ZeroAddr,
		reversed:    false,
		int0:        int0,
		int1:        int1,
		bigPrice:    bigPrice,
		price:       *bigPrice,
		source:      source,
	}
	r.report("using Maker")
	return *bigPrice, source, err
}

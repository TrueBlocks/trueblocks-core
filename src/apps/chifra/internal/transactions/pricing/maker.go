package pricing

import (
	"fmt"
	"math/big"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/call"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var (
	makerMedianizer = base.HexToAddress("0x729d19f657bd0614b4985cf1d82531c67569197b")
	makerDeployment = base.Blknum(3684349)
)

func PriceUsdMaker(chain string, testMode bool, statement *types.SimpleStatement) (price float64, source string, err error) {
    // TODO: BOGUS - THIS COULD BE DONE IN THE CALLER
	if statement.BlockNumber <= makerDeployment {
		msg := fmt.Sprintf("Block %d is prior to deployment (%d) of Maker. No fallback pricing method", statement.BlockNumber, makerDeployment)
		logger.TestLog(true, msg)
		return 0.0, "eth-not-priced-pre-maker", nil
	}

	msg := fmt.Sprintf("Block %d is prior to deployment (%d) of Uniswap V2. Falling back to Maker (%s)", statement.BlockNumber, uniswapFactoryV2_deployed, makerMedianizer)
	logger.TestLog(true, msg)
	theCall := "peek()"
	contractCall, err := call.NewContractCall(chain, makerMedianizer, theCall, false)
	if err != nil {
		return 0.0, "not-priced", err
	}

	contractCall.BlockNumber = statement.BlockNumber
	result, err := call.CallContract(chain, contractCall)
	if err != nil {
		return 0.0, "not-priced", err
	}

	divisor := new(big.Int)
	divisor.SetString("1000000000000000000", 10)

    // TODO: BOGUS - THIS SHOULD USE THE RESULT VALUE RETURNED INSTEAD OF PEEKING AT RAWDATA
	rawHex := strings.TrimPrefix(string(result.RawReturn), "0x")
	rawHex = rawHex[:64]
	int0 := new(big.Int)
	int0.SetString(rawHex, 16)
	int0 = int0.Mul(int0, new(big.Int).SetInt64(100000))
	int1 := new(big.Int).Quo(int0, divisor)

	bigPrice := new(big.Float).SetInt(int1)
	bigPrice = bigPrice.Quo(bigPrice, new(big.Float).SetInt64(100000))
	price, _ = bigPrice.Float64()
	source = "maker"
	r := reporter{
		address:     statement.AssetAddr,
		symbol:      statement.AssetSymbol,
		blockNumber: statement.BlockNumber,
		source1:     makerMedianizer,
		theCall1:    theCall,
		source2:     base.HexToAddress("0x0"),
		theCall2:    "0x" + rawHex,
		first:       base.HexToAddress("0x0"),
		second:      base.HexToAddress("0x0"),
		reversed:    false,
		int0:        int0,
		int1:        int1,
		bigPrice:    bigPrice,
		price:       price,
		source:      source,
	}
	r.report("using Maker", testMode)

	return
}
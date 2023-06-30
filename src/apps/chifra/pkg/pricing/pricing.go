package pricing

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: Much of this reporting could be removed as it's only used for debugging

// PriceUsd returns the price of the asset in USD
func PriceUsd(chain string, testMode bool, statement *types.SimpleStatement) (price float64, source string, err error) {
	if statement.IsStableCoin() {
		r := reporter{
			address: statement.AssetAddr,
			symbol:  statement.AssetSymbol,
		}
		r.report("stable-coin", testMode)
		return 1.0, "stable-coin", nil
	}

	if statement.BlockNumber <= uniswapFactoryV2_deployed {
		if statement.IsEth() {
			return PriceUsdMaker(chain, testMode, statement)
		} else {
			msg := fmt.Sprintf("Block %d is prior to deployment (%d) of Uniswap V2. No other source for tokens prior to UniSwap", statement.BlockNumber, uniswapFactoryV2_deployed)
			logger.TestLog(true, msg)
			return 0.0, "token-not-priced-pre-uni", nil
		}
	}

	return PriceUsdUniswap(chain, testMode, statement)
}

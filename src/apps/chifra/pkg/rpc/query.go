package rpc

import (
	"context"
	"fmt"
	"math/big"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/ethclient"
)

// TODO: DUPLICATED DUE TO CYCLICAL IMPORT
var perProviderClientMap = map[string]*ethclient.Client{}
var clientMutex sync.Mutex

func getClient(chain string) (*ethclient.Client, error) {
	provider, _ := config.GetRpcProvider(chain)
	if provider == "https://" {
		var noProvider string = `

  Warning: The RPC server ([{PROVIDER}]) was not available. Either start it, or edit the rpcProvider
  value in the file [{FILE}]. Quitting...
`
		msg := noProvider
		msg = strings.Replace(msg, "[{PROVIDER}]", provider, -1)
		msg = strings.Replace(msg, "[{FILE}]", config.GetPathToRootConfig()+"trueBlocks.toml", -1)
		msg = strings.Replace(msg, "https://", "<empty>", -1)
		return nil, fmt.Errorf("%s", msg)
	}

	clientMutex.Lock()
	if perProviderClientMap[provider] == nil {
		ec, err := ethclient.Dial(provider)
		if err != nil || ec == nil {
			logger.Error("Missdial("+provider+"):", err)
			logger.Fatal("")
		}
		perProviderClientMap[provider] = ec
	}
	clientMutex.Unlock()
	return perProviderClientMap[provider], nil
}

// GetBlockTimestamp returns the timestamp associated with a given block
func GetBlockTimestamp(chain string, bn *uint64) base.Timestamp {
	if ec, err := getClient(chain); err != nil {
		logger.Error("Could not connect to RPC client", err)
		return 0
	} else {
		defer ec.Close()

		var blockNumber *big.Int
		if bn != nil {
			blockNumber = big.NewInt(int64(*bn))
		}

		r, err := ec.HeaderByNumber(context.Background(), blockNumber)
		if err != nil {
			logger.Error("Could not connect to RPC client", err)
			return 0
		}

		ts := base.Timestamp(r.Time)
		if ts == 0 {
			// The RPC does not return a timestamp for block zero, so we simulate it with ts from block one less 13 seconds
			// TODO: Chain specific
			return GetBlockTimestamp(chain, utils.PointerOf(uint64(1))) - 13
		}

		return ts
	}
}

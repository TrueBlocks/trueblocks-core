package rpcClient

import (
	"context"
	"errors"
	"math/big"
	"sort"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
)

var ErrNotAContract = errors.New("not a contract")

// IsContractAt checks if an account is a contract
func (options *Options) IsContractAt(address base.Address, block *types.SimpleNamedBlock) error {
	if ec, err := getClient(options.Chain); err != nil {
		return err
	} else {
		defer ec.Close()

		var clientBlockArg *big.Int = nil
		if block != nil && block.Name != "latest" {
			clientBlockArg = big.NewInt(0).SetUint64(block.BlockNumber)
		}

		ctx := context.Background()
		if code, err := ec.CodeAt(ctx, address.ToCommon(), clientBlockArg); err != nil {
			return err
		} else {
			if len(code) == 0 {
				return ErrNotAContract
			}
		}
		return nil
	}
}

var deployedCacheMutex sync.Mutex
var deployedCache = make(map[base.Address]base.Blknum)

func (options *Options) GetContractDeployBlock(address base.Address) (block base.Blknum, err error) {
	// TODO: Couldn't we wait here to lock until we need it? Doesn't this lock even when we only read the cache?
	deployedCacheMutex.Lock()
	defer deployedCacheMutex.Unlock()

	if cached, ok := deployedCache[address]; ok {
		block = cached
		return
	}

	latest := options.GetLatestBlockNumber()
	if err = options.IsContractAt(address, &types.SimpleNamedBlock{BlockNumber: latest}); err != nil {
		return
	}

	found := sort.Search(int(latest)+1, func(blockNumber int) bool {
		err := options.IsContractAt(address, &types.SimpleNamedBlock{BlockNumber: base.Blknum(blockNumber)})
		return err == nil
	})

	block = base.Blknum(found)
	deployedCache[address] = block
	return
}

// We check a bunch of different locations for the proxy
var locations = []string{
	"0x360894a13ba1a3210667c828492db98dca3e2076cc3735a920a3ca505d382bbc", // EIP1967
	"0x7050c9e0f4ca769c69bd3a8ef740bc37934f8e2c036e5a723fd8ee048ed3f8c3", // EIP1967ZOS
	"0xc5f16f0fcc639fa48a6947836d9850f504798523bf8c9a3a87d5876cf622bcf7", // EIP1822
	"0x5f3b5dfeb7b28cdbd7faba78963ee202a494e2a2cc8c9978d5e30d2aebb8c197", // EIP1822ZOS};
	"0x",
}

// GetProxyAt returns the proxy address for a contract if any
func (options *Options) GetProxyAt(address base.Address, blockNumber base.Blknum) (base.Address, error) {
	if ec, err := getClient(options.Chain); err != nil {
		return base.Address{}, err
	} else {
		defer ec.Close()

		proxyAddr, err := rpc.Query[string](options.Chain, "eth_call", rpc.Params{
			map[string]any{
				"to": address,
				// implementation()
				"data": "0x59679b0f",
			},
			blockNumber,
		})
		var proxy base.Address
		if proxyAddr != nil {
			proxy = base.HexToAddress(*proxyAddr)
		}
		if err == nil && !proxy.IsZero() && proxy.Hex() != address.Hex() {
			return proxy, err
		}

		bn := big.NewInt(0).SetUint64(blockNumber)

		for _, location := range locations {
			var value []byte
			value, err = ec.StorageAt(
				context.Background(),
				address.Address,
				common.HexToHash(location),
				bn,
			)
			if err != nil {
				return proxy, err
			}
			proxy = base.BytesToAddress(value)
			if !proxy.IsZero() && proxy.Hex() != address.Hex() {
				err = options.IsContractAt(proxy, &types.SimpleNamedBlock{BlockNumber: blockNumber})
				if errors.Is(err, ErrNotAContract) {
					// Not a proxy
					return base.Address{}, nil
				}
				return proxy, err
			}
			proxy = base.Address{}
		}

		return proxy, err
	}
}

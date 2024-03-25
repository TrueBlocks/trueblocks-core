package rpc

import (
	"context"
	"errors"
	"sort"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common"
)

var ErrNotAContract = errors.New("not a contract")

// IsContractAt checks if an account is a contract
func (conn *Connection) IsContractAtLatest(address base.Address) error {
	return conn.IsContractAt(address, utils.NOPOS)
}

// IsContractAt checks if an account is a contract
func (conn *Connection) IsContractAt(address base.Address, bn uint64) error {
	if ec, err := conn.getClient(); err != nil {
		return err
	} else {
		defer ec.Close()

		ctx := context.Background()
		if code, err := ec.CodeAt(ctx, address.Common(), base.BiFromUint64(bn)); err != nil {
			return err
		} else {
			if len(code) == 0 {
				return ErrNotAContract
			}
		}
		return nil
	}
}

// GetContractCodeAt returns a code (if any) for an address at a block
func (conn *Connection) GetContractCodeAt(addr base.Address, bn uint64) ([]byte, error) {
	if ec, err := conn.getClient(); err != nil {
		return []byte{}, err
	} else {
		defer ec.Close()
		return ec.CodeAt(context.Background(), addr.Common(), base.BiFromUint64(bn))
	}
}

// We check a bunch of different locations for the proxy
var locations = []string{
	"0x360894a13ba1a3210667c828492db98dca3e2076cc3735a920a3ca505d382bbc", // EIP1967
	"0x7050c9e0f4ca769c69bd3a8ef740bc37934f8e2c036e5a723fd8ee048ed3f8c3", // EIP1967ZOS
	"0xc5f16f0fcc639fa48a6947836d9850f504798523bf8c9a3a87d5876cf622bcf7", // EIP1822
	"0x5f3b5dfeb7b28cdbd7faba78963ee202a494e2a2cc8c9978d5e30d2aebb8c197", // EIP1822ZOS};
	"0x",
}

// GetContractProxyAt returns the proxy address for a contract if any
func (conn *Connection) GetContractProxyAt(address base.Address, blockNumber base.Blknum) (base.Address, error) {
	if ec, err := conn.getClient(); err != nil {
		return base.Address{}, err
	} else {
		defer ec.Close()

		method := "eth_call"
		params := query.Params{
			map[string]any{
				"to": address,
				// implementation()
				"data": "0x59679b0f",
			},
			blockNumber,
		}

		proxyAddr, err := query.Query[string](conn.Chain, method, params)
		var proxy base.Address
		if proxyAddr != nil {
			proxy = base.HexToAddress(*proxyAddr)
		}
		if err == nil && !proxy.IsZero() && proxy.Hex() != address.Hex() {
			return proxy, err
		}

		for _, location := range locations {
			var value []byte
			value, err = ec.StorageAt(
				context.Background(),
				address.Address,
				common.HexToHash(location),
				base.BiFromUint64(blockNumber),
			)
			if err != nil {
				return proxy, err
			}
			proxy = base.BytesToAddress(value)
			if !proxy.IsZero() && proxy.Hex() != address.Hex() {
				err = conn.IsContractAt(proxy, blockNumber)
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

// TODO: We could use a SyncMap here
var deployedCacheMutex sync.Mutex
var deployedCache = make(map[base.Address]base.Blknum)

func (conn *Connection) GetContractDeployBlock(address base.Address) (block base.Blknum, err error) {
	// TODO: Couldn't we wait here to lock until we need it? Doesn't this lock even when we only read the cache?
	deployedCacheMutex.Lock()
	defer deployedCacheMutex.Unlock()

	if cached, ok := deployedCache[address]; ok {
		block = cached
		return
	}

	latest := conn.GetLatestBlockNumber()
	if err = conn.IsContractAt(address, latest); err != nil {
		return
	}

	found := sort.Search(int(latest)+1, func(bn int) bool {
		err := conn.IsContractAt(address, uint64(bn))
		return err == nil
	})

	block = base.Blknum(found)
	deployedCache[address] = block
	return
}

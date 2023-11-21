package abi

import (
	"sync"
	"sync/atomic"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type SelectorSyncMap struct {
	sync.Map
}

func (abiMap *SelectorSyncMap) GetValue(encoding string) *types.SimpleFunction {
	if function, ok := abiMap.Load(encoding); !ok {
		return nil
	} else {
		return function.(*types.SimpleFunction)
	}
}

func (abiMap *SelectorSyncMap) SetValue(encoding string, function *types.SimpleFunction) {
	abiMap.Store(encoding, function)
}

func (abiMap *SelectorSyncMap) Count() int64 {
	var cnt atomic.Int64
	countFunc := func(k any, b any) bool {
		cnt.Add(1)
		return true
	}
	abiMap.Range(countFunc)
	return cnt.Load()
}

func (abiMap *SelectorSyncMap) Keys() []string {
	ret := make([]string, 0, abiMap.Count())
	visit := func(k any, b any) bool {
		name, _ := k.(string)
		ret = append(ret, name)
		return true
	}
	abiMap.Range(visit)
	return ret
}

func (abiMap *SelectorSyncMap) Values() []types.SimpleFunction {
	ret := make([]types.SimpleFunction, 0, abiMap.Count())
	visit := func(k any, b any) bool {
		function, _ := b.(*types.SimpleFunction)
		ret = append(ret, *function)
		return true
	}
	abiMap.Range(visit)
	return ret
}

type AddressSyncMap struct {
	sync.Map
}

func (addrMap *AddressSyncMap) GetValue(addr base.Address) bool {
	if set, ok := addrMap.Load(addr); !ok {
		return false
	} else {
		return set.(bool)
	}
}

func (addrMap *AddressSyncMap) SetValue(addr base.Address, set bool) {
	addrMap.Store(addr, set)
}

func (addrMap *AddressSyncMap) Count() int64 {
	var cnt atomic.Int64
	countFunc := func(k any, b any) bool {
		cnt.Add(1)
		return true
	}
	addrMap.Range(countFunc)
	return cnt.Load()
}

func (addrMap *AddressSyncMap) Keys() []base.Address {
	ret := make([]base.Address, 0, addrMap.Count())
	visit := func(k any, b any) bool {
		addr, _ := k.(base.Address)
		ret = append(ret, addr)
		return true
	}
	addrMap.Range(visit)
	return ret
}

func (addrMap *AddressSyncMap) Values() []bool {
	ret := make([]bool, 0, addrMap.Count())
	visit := func(k any, b any) bool {
		set, _ := b.(bool)
		ret = append(ret, set)
		return true
	}
	addrMap.Range(visit)
	return ret
}

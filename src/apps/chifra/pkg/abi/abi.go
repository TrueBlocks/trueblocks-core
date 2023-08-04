package abi

import (
	"sync"
	"sync/atomic"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type FunctionSyncMap struct {
	sync.Map
}

func (f *FunctionSyncMap) SetValue(encoding string, function *types.SimpleFunction) {
	f.Store(encoding, function)
}

func (f *FunctionSyncMap) GetValue(encoding string) *types.SimpleFunction {
	if function, ok := f.Load(encoding); !ok {
		return nil
	} else {
		return function.(*types.SimpleFunction)
	}
}

func NewFunctionSyncMap() *FunctionSyncMap {
	return &FunctionSyncMap{}
}

func (abiMap *FunctionSyncMap) Count() int64 {
	var cnt atomic.Int64
	countFunc := func(k any, b any) bool {
		cnt.Add(1)
		return true
	}
	abiMap.Range(countFunc)
	return cnt.Load()
}

func (abiMap *FunctionSyncMap) Keys() []string {
	ret := make([]string, 0, abiMap.Count())
	visit := func(k any, b any) bool {
		name, _ := k.(string)
		ret = append(ret, name)
		return true
	}
	abiMap.Range(visit)
	return ret
}

func (abiMap *FunctionSyncMap) Values() []types.SimpleFunction {
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

func (f *AddressSyncMap) SetValue(addr base.Address, set bool) {
	f.Store(addr, set)
}

func (f *AddressSyncMap) GetValue(addr base.Address) bool {
	if set, ok := f.Load(addr); !ok {
		return false
	} else {
		return set.(bool)
	}
}

func NewAddressSyncMap() *AddressSyncMap {
	return &AddressSyncMap{}
}

func (abiMap *AddressSyncMap) Count() int64 {
	var cnt atomic.Int64
	countFunc := func(k any, b any) bool {
		cnt.Add(1)
		return true
	}
	abiMap.Range(countFunc)
	return cnt.Load()
}

func (abiMap *AddressSyncMap) Keys() []base.Address {
	ret := make([]base.Address, 0, abiMap.Count())
	visit := func(k any, b any) bool {
		addr, _ := k.(base.Address)
		ret = append(ret, addr)
		return true
	}
	abiMap.Range(visit)
	return ret
}

func (abiMap *AddressSyncMap) Values() []bool {
	ret := make([]bool, 0, abiMap.Count())
	visit := func(k any, b any) bool {
		set, _ := b.(bool)
		ret = append(ret, set)
		return true
	}
	abiMap.Range(visit)
	return ret
}

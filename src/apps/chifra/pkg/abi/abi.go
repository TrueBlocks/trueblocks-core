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

func (f *FunctionSyncMap) Count() int64 {
	var cnt atomic.Int64
	countFunc := func(k any, b any) bool {
		cnt.Add(1)
		return true
	}
	f.Range(countFunc)
	return cnt.Load()
}

func (f *FunctionSyncMap) Keys() []string {
	ret := make([]string, 0, f.Count())
	visit := func(k any, b any) bool {
		name, _ := k.(string)
		ret = append(ret, name)
		return true
	}
	f.Range(visit)
	return ret
}

func (f *FunctionSyncMap) Values() []types.SimpleFunction {
	ret := make([]types.SimpleFunction, 0, f.Count())
	visit := func(k any, b any) bool {
		function, _ := b.(*types.SimpleFunction)
		ret = append(ret, *function)
		return true
	}
	f.Range(visit)
	return ret
}

type AddressSyncMap struct {
	sync.Map
}

func (a *AddressSyncMap) SetValue(addr base.Address, set bool) {
	a.Store(addr, set)
}

func (a *AddressSyncMap) GetValue(addr base.Address) bool {
	if set, ok := a.Load(addr); !ok {
		return false
	} else {
		return set.(bool)
	}
}

func NewAddressSyncMap() *AddressSyncMap {
	return &AddressSyncMap{}
}

func (a *AddressSyncMap) Count() int64 {
	var cnt atomic.Int64
	countFunc := func(k any, b any) bool {
		cnt.Add(1)
		return true
	}
	a.Range(countFunc)
	return cnt.Load()
}

func (a *AddressSyncMap) Keys() []base.Address {
	ret := make([]base.Address, 0, a.Count())
	visit := func(k any, b any) bool {
		addr, _ := k.(base.Address)
		ret = append(ret, addr)
		return true
	}
	a.Range(visit)
	return ret
}

func (a *AddressSyncMap) Values() []bool {
	ret := make([]bool, 0, a.Count())
	visit := func(k any, b any) bool {
		set, _ := b.(bool)
		ret = append(ret, set)
		return true
	}
	a.Range(visit)
	return ret
}

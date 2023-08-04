package abi

import (
	"sync"
	"sync/atomic"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type FunctionSyncMap12 struct {
	sync.Map
}

func (f *FunctionSyncMap12) Set(encoding string, function *types.SimpleFunction) {
	f.Store(encoding, function)
}

func (f *FunctionSyncMap12) Get(encoding string) *types.SimpleFunction {
	if function, ok := f.Load(encoding); !ok {
		return nil
	} else {
		return function.(*types.SimpleFunction)
	}
}

func NewFunctionSyncMap12() *FunctionSyncMap12 {
	return &FunctionSyncMap12{}
}

func (abiMap *FunctionSyncMap12) Count() int64 {
	var cnt atomic.Int64
	countFunc := func(k any, b any) bool {
		cnt.Add(1)
		return true
	}
	abiMap.Range(countFunc)
	return cnt.Load()
}

func (abiMap *FunctionSyncMap12) Functions() []types.SimpleFunction {
	ret := make([]types.SimpleFunction, 0, abiMap.Count())
	visit := func(k any, b any) bool {
		function, _ := b.(*types.SimpleFunction)
		ret = append(ret, *function)
		return true
	}
	abiMap.Range(visit)
	return ret
}

func (abiMap *FunctionSyncMap12) Names() []string {
	ret := make([]string, 0, abiMap.Count())
	visit := func(k any, b any) bool {
		name, _ := k.(string)
		ret = append(ret, name)
		return true
	}
	abiMap.Range(visit)
	return ret
}

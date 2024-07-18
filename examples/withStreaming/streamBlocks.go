package main

import (
	"fmt"
	"reflect"

	"github.com/TrueBlocks/trueblocks-core/sdk/v3"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type BlockType interface {
	*types.Block | *types.LightBlock | *types.Log | *types.Trace | *types.Appearance | *types.Withdrawal
}

func TestBlocks[T BlockType](mode ...string) {
	opts := sdk.BlocksOptions{
		BlockIds:  []string{"3-20000003:1000000"},
		RenderCtx: output.NewStreamingContext(),
	}

	cnt := 0
	go func() {
		for {
			select {
			case model := <-opts.RenderCtx.ModelChan:
				if item, ok := model.(T); ok {
					fmt.Printf("%d item: %s\r", cnt, item)
				} else {
					logger.Error("not the right type", reflect.TypeOf(model))
				}
			case err := <-opts.RenderCtx.ErrorChan:
				logger.Error(err.Error())
			}
			cnt++
			if opts.RenderCtx.WasCanceled() {
				fmt.Println()
				return
			}
		}
	}()

	var v T
	switch any(v).(type) {
	case *types.Block:
		if len(mode) > 0 && mode[0] == "uncles" {
			if _, _, err := opts.BlocksUncles(); err != nil {
				logger.Error(err.Error())
			}
			// } else {
			// 	if _, _, err := opts.Blocks(); err != nil {
			// 		logger.Error(err.Error())
			// 	}
		}
	case *types.LightBlock:
		if _, _, err := opts.BlocksHashes(); err != nil {
			logger.Error(err.Error())
		}
	case *types.Log:
		if _, _, err := opts.BlocksLogs(); err != nil {
			logger.Error(err.Error())
		}
	case *types.Trace:
		if _, _, err := opts.BlocksTraces(); err != nil {
			logger.Error(err.Error())
		}
	case *types.Appearance:
		if _, _, err := opts.BlocksUniq(); err != nil {
			logger.Error(err.Error())
		}
	case *types.Withdrawal:
		if _, _, err := opts.BlocksWithdrawals(); err != nil {
			logger.Error(err.Error())
		}
	}
	opts.RenderCtx.Cancel()
}

func TestStreamBlocks() {
	TestBlocks[*types.Block]()
	TestBlocks[*types.LightBlock]()
	TestBlocks[*types.Log]()
	TestBlocks[*types.Trace]()
	TestBlocks[*types.Block]("uncles")
	// TestBlocks[*types.Appearance]()
	TestBlocks[*types.Withdrawal]()
}

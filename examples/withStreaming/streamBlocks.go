package main

import (
	"fmt"
	"reflect"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type BlockType interface {
	*types.Block | *types.LightBlock | *types.Log | *types.Trace | *types.Appearance | *types.Withdrawal
}

func TestBlocks[T BlockType](str string) {
	opts := sdk.BlocksOptions{
		BlockIds:  []string{"14000000-14000100:25"},
		RenderCtx: output.WithChannels(),
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

	switch str {
	case "block":
		if _, _, err := opts.Blocks(); err != nil {
			logger.Error(err.Error())
		}
	case "lightblock":
		if _, _, err := opts.BlocksHashes(); err != nil {
			logger.Error(err.Error())
		}
	case "logs":
		if _, _, err := opts.BlocksLogs(); err != nil {
			logger.Error(err.Error())
		}
	case "traces":
		if _, _, err := opts.BlocksTraces(); err != nil {
			logger.Error(err.Error())
		}
	case "uncles":
		if _, _, err := opts.BlocksUncles(); err != nil {
			logger.Error(err.Error())
		}
	case "uniq":
		if _, _, err := opts.BlocksUniq(); err != nil {
			logger.Error(err.Error())
		}
	case "withdrawals":
		if _, _, err := opts.BlocksWithdrawals(); err != nil {
			logger.Error(err.Error())
		}
	}
	opts.RenderCtx.Cancel()
}

func TestStreamBlocks() {
	TestBlocks[*types.Block]("block")
	TestBlocks[*types.LightBlock]("lightblock")
	TestBlocks[*types.Log]("logs")
	TestBlocks[*types.Trace]("traces")
	TestBlocks[*types.Block]("uncles")
	// TestBlocks[*types.Appearance]("uniq")
	TestBlocks[*types.Withdrawal]("withdrawals")
}

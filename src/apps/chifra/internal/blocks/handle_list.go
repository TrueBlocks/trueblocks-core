// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

func (opts *BlocksOptions) HandleList() error {
	// meta, err := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
	// if err != nil {
	// 	return err
	// }
	// if opts.Globals.TestMode {
	// 	meta.Latest = 2000100
	// }
	// start := meta.Latest - opts.List
	// end := start - opts.ListCount
	// if start < opts.ListCount {
	// 	end = 0
	// }

	// ctx, cancel := context.WithCancel(context.Background())

	// fetchData := func(modelChan chan types.Modeler[types.RawBlock], errorChan chan error) {
	// 	for bn := meta.Latest - opts.List; bn > end; bn-- {
	// 		var block types.SimpleBlock
	// 		if block, err = rpcClient.GetBlockByNumber(opts.Globals.Chain, bn, false); err != nil {
	// 			errorChan <- err
	// 			cancel()
	// 			return
	// 		} else {
	// 			modelChan <- &block
	// 		}
	// 	}
	// }

	// return output.StreamMany(ctx, fetchData, output.OutputOptions{
	// 	Writer:     opts.Globals.Writer,
	// 	Chain:      opts.Globals.Chain,
	// 	TestMode:   opts.Globals.TestMode,
	// 	NoHeader:   opts.Globals.NoHeader,
	// 	ShowRaw:    opts.Globals.ShowRaw,
	// 	Verbose:    opts.Globals.Verbose,
	// 	LogLevel:   opts.Globals.LogLevel,
	// 	Format:     opts.Globals.Format,
	// 	OutputFn:   opts.Globals.OutputFn,
	// 	Append:     opts.Globals.Append,
	// 	JsonIndent: "  ",
	// })
	return nil
}

/*
   CBlock block;
   getBlockLight(block, b);

   How to use StreamMany
   copy from handle_ts_count.go

	fetchData := func(modelChan chan types.Modeler[types.RawWhenCount], errorChan chan error) {
		if count, err := tslib.NTimestamps(opts.Globals.Chain); err != nil {
			errorChan <- err
			cancel()
			return
		} else {
			if opts.Globals.TestMode {
				count = 5000000
			}
			modelChan <- &types.SimpleWhenCount{Count: count}
		}
	}

	return output.StreamMany(ctx, fetchData, output.OutputOptions{
		Writer:     opts.Globals.Writer,
		Chain:      opts.Globals.Chain,
		TestMode:   opts.Globals.TestMode,
		NoHeader:   opts.Globals.NoHeader,
		ShowRaw:    opts.Globals.ShowRaw,
		Verbose:    opts.Globals.Verbose,
		LogLevel:   opts.Globals.LogLevel,
		Format:     opts.Globals.Format,
		OutputFn:   opts.Globals.OutputFn,
		Append:     opts.Globals.Append,
		JsonIndent: "  ",
	})
}
*/

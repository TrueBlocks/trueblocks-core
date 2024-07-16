// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package abisPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common/hexutil"
	"github.com/ethereum/go-ethereum/crypto"
)

func (opts *AbisOptions) HandleEncode(rCtx *output.RenderCtx) error {
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		funcs := abi.ExtractSigs(opts.Encode)
		if len(funcs) == 0 {
			errorChan <- fmt.Errorf("not a valid function signature in HandleEncode: %s", opts.Encode)
			return
		}
		for _, f := range funcs {
			enc := hexutil.Encode(crypto.Keccak256([]byte(f.Signature)))
			if f.FunctionType == "function" {
				enc = enc[:10]
			}
			f.Encoding = enc
			modelChan <- &f
		}
	}
	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package abisPkg

import (
	"context"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common/hexutil"
	"github.com/ethereum/go-ethereum/crypto"
)

func (opts *AbisOptions) HandleEncode() error {
	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawFunction], errorChan chan error) {
		parts := strings.Split(opts.Encode, "(")
		if len(parts) == 0 {
			parts = append(parts, opts.Encode)
		}
		function := types.Function{
			Encoding:     hexutil.Encode(crypto.Keccak256([]byte(opts.Encode))),
			Name:         parts[0],
			Signature:    opts.Encode,
			FunctionType: "function",
		}
		modelChan <- &function
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

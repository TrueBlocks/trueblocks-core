// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package abisPkg

import (
	"fmt"

	"github.com/ethereum/go-ethereum/common/hexutil"
	"github.com/ethereum/go-ethereum/crypto"
)

func (opts *AbisOptions) HandleEncode() error {
	fmt.Println(opts.Encode, "-->", hexutil.Encode(crypto.Keccak256([]byte(opts.Encode))))
	return nil
}

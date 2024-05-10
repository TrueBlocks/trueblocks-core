// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tokensPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func (opts *TokensOptions) HandleDecache() error {
	// TODO: decache.Decache --decache not implemented
	logger.Error("chifra tokens --decache is not yet implemented")
	return nil
}

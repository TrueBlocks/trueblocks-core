// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tokensPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

func (opts *TokensOptions) HandleDecache(rCtx output.RenderCtx) error {
	// TODO: decache.Decache --decache not implemented
	_ = context.TODO()
	logger.Error("chifra tokens --decache is not yet implemented")
	return nil
}

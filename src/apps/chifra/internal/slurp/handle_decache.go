// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package slurpPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

func (opts *SlurpOptions) HandleDecache(rCtx *output.RenderCtx) error {
	_ = rCtx
	// TODO: decache.Decache --decache is not yet implemented
	logger.Error("chifra slurp --decache is not yet implemented")
	return nil
}

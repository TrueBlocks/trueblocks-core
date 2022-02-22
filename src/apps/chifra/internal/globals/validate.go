// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *GlobalOptions) ValidateGlobals() error {
	if len(opts.File) > 0 && !file.FileExists(opts.File) {
		return validate.Usage("The {0} option ({1}) must {2}", "file", opts.File, "exist")
	}

	err := validate.ValidateEnum("--fmt", opts.Format, "[json|txt|csv|api]")
	if err != nil {
		return err
	}
	return nil
}

// TODO: This could be removed by changing the code generator
func ToFloat64(val string) float64 {
	f, _ := strconv.ParseFloat(val, 64)
	return f
}

// TODO: This could be removed by changing the code generator
func ToUint64(val string) uint64 {
	f, _ := strconv.ParseUint(val, 10, 64)
	return f
}

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package listPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ListOptions) ValidateList() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	err := validate.ValidateAtLeastOneAddr(opts.Addrs)
	if err != nil {
		return err
	}

	return nil
}

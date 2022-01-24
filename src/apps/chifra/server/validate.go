// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package servePkg

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

func Validate(cmd *cobra.Command, args []string) error {
	if len(GetOptions().Port) > 0 && !strings.Contains(GetOptions().Port, ":") {
		return validate.Usage("The {0} option ({1}) must {2}.", "--port", GetOptions().Port, "start with ':'")
	}

	return nil
}

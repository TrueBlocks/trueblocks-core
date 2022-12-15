// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cmd

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/spf13/cobra"
)

var statusCmd = &cobra.Command{
	Use:     usageStatus,
	Short:   shortStatus,
	Long:    longStatus,
	Version: versionText,
	RunE: func(cmd *cobra.Command, args []string) error {
		fmt.Println(colors.BrightYellow, "\n\tThe `chifra status` command has been renamed to `chifra config`.\n", colors.Off)
		return nil
	},
}

const usageStatus = `status`

const shortStatus = "deprecated"

const longStatus = "deprecated"

func init() {
	statusCmd.SetOut(os.Stderr)
	chifraCmd.AddCommand(statusCmd)
}

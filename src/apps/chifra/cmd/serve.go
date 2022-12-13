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

var serveCmd = &cobra.Command{
	Use:     usageServe,
	Short:   shortServe,
	Long:    longServe,
	Version: versionText,
	RunE: func(cmd *cobra.Command, args []string) error {
		fmt.Println(colors.BrightYellow, "\n\tThe `chifra serve` command has been renamed to `chifra daemon`.\n", colors.Off)
		return nil
	},
}

const usageServe = `serve`

const shortServe = "deprecated"

const longServe = "deprecated"

func init() {
	serveCmd.SetOut(os.Stderr)
	chifraCmd.AddCommand(serveCmd)
}

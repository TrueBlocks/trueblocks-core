// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cmd

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
	"github.com/spf13/cobra"
)

// Make a copy of this so we don't have to import the version package in every file
const versionText = version.LibraryVersion

// This allows the user to type chifra version and get the version number. Previously, each
// tool had a --version option (and still do), but this is closer to expected behaviour.
var versionCmd = &cobra.Command{
	Use:   "version",
	Short: "Print the version number of Chifra",
	Long:  `All software has versions. This is Chifra's`,
	Run: func(cmd *cobra.Command, args []string) {
		_ = cmd
		_ = args
		fmt.Println(versionText)
	},
}

func init() {
	chifraCmd.AddCommand(versionCmd)
}

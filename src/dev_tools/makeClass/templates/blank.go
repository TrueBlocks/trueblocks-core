// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package cmd

// EXISTING_CODE
// EXISTING_CODE

// [{ROUTE}]Cmd represents the [{ROUTE}] command
var [{ROUTE}]Cmd = &cobra.Command{
	Use:     usage[{PROPER}],
	Short:   short[{PROPER}],
	Long:    long[{PROPER}],
	Version: versionText,
	RunE:    [{ROUTE}]Pkg.Run[{PROPER}],
}

var usage[{PROPER}] = `[{USE}]`

var short[{PROPER}] = "[{SHORT}]"

var long[{PROPER}] = `[{LONG}]`

var notes[{PROPER}] = `[{POSTNOTES}]`

[{OPT_DEF}]func init() {
	[{ROUTE}]Cmd.Flags().SortFlags = false

[{SET_OPTS}][{HIDDEN}]	globals.InitGlobals([{ROUTE}]Cmd, &[{ROUTE}]Pkg.Options.Globals)

	[{ROUTE}]Cmd.SetUsageTemplate(UsageWithNotes(notes[{PROPER}]))
	[{ROUTE}]Cmd.SetOut(os.Stderr)

	chifraCmd.AddCommand([{ROUTE}]Cmd)
}

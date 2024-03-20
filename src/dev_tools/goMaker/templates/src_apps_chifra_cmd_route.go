// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package cmd

// EXISTING_CODE
// EXISTING_CODE

// {{.Route}}Cmd represents the {{.Route}} command
var {{.Route}}Cmd = &cobra.Command{
	Use:     usage{{.Proper}},
	Short:   short{{.Proper}},
	Long:    long{{.Proper}},
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("{{.Route}}", func() *globals.GlobalOptions {
		return &{{.Route}}Pkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("{{.Route}}", {{.Route}}Pkg.Run{{.Proper}}, {{.Route}}Pkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &{{.Route}}Pkg.GetOptions().Globals
	}),
{{.AliasStr}}}

const usage{{.Proper}} = `{{.Usage}}{{.Use}}`

const short{{.Proper}} = "{{.Short}}"

const long{{.Proper}} = `{{.Long}}`

const notes{{.Proper}} = `{{.NotesStr}}`

{{.OptDef}}func init() {
	{{.Capabilities}}

	{{.Route}}Cmd.Flags().SortFlags = false

{{.SetOptions}}{{.HiddenStr}}	globals.InitGlobals("{{.Route}}", {{.Route}}Cmd, &{{.Route}}Pkg.GetOptions().Globals, capabilities)

	{{.Route}}Cmd.SetUsageTemplate(UsageWithNotes(notes{{.Proper}}))
	{{.Route}}Cmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand({{.Route}}Cmd)
}

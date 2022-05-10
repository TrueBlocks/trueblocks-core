// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package [{API_ROUTE}]Pkg

// EXISTING_CODE
// EXISTING_CODE

// Run[{PROPER}] handles the [{ROUTE}] command for the command line. Returns error only as per cobra.
func Run[{PROPER}](cmd *cobra.Command, args []string) (err error) {
	opts := [{PROPER}]FinishParse(args)
	// JINKY
	// JINKY
	err, _ = opts.[{PROPER}]Internal()
	return
}

// Serve[{PROPER}] handles the [{ROUTE}] command for the API. Returns error and a bool if handled
func Serve[{PROPER}](w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := [{PROPER}]FinishParseApi(w, r)
	// JINKY
	// JINKY
	return opts.[{PROPER}]Internal()
}

// [{PROPER}]Internal handles the internal workings of the [{ROUTE}] command.  Returns error and a bool if handled
func (opts *[{PROPER}]Options) [{PROPER}]Internal() (err error, handled bool) {
	err = opts.Validate[{PROPER}]()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE

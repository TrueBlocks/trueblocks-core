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

func Run[{PROPER}](cmd *cobra.Command, args []string) error {
	opts := [{PROPER}]FinishParse(args)

	err := opts.Validate[{PROPER}]()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	// EXISTING_CODE
}

func Serve[{PROPER}](w http.ResponseWriter, r *http.Request) bool {
	opts := FromRequest(w, r)

	err := opts.Validate[{PROPER}]()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE

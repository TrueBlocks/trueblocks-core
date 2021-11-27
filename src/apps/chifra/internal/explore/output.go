// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package explorePkg

// EXISTING_CODE
import (
	"fmt"
	"net/http"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

func RunExplore(cmd *cobra.Command, args []string) error {
	opts := ExploreFinishParse(args)

	err := opts.ValidateExplore()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	for _, url := range urls {
		fmt.Printf("Opening %s\n", url.getUrl())
		if os.Getenv("TEST_MODE") != "true" {
			utils.OpenBrowser(url.getUrl())
		}
	}

	return nil
	// EXISTING_CODE
}

func ServeExplore(w http.ResponseWriter, r *http.Request) bool {
	opts := FromRequest(w, r)

	err := opts.ValidateExplore()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	return false
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE

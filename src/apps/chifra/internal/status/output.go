// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package statusPkg

// EXISTING_CODE
import (
	"net/http"

	// "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/spf13/cobra"
)

// func GreenTxt(txt string) string {
// 	return colors.Green + txt + colors.Off
// }

// EXISTING_CODE

func RunStatus(cmd *cobra.Command, args []string) error {
	opts := StatusFinishParse(args)

	err := opts.ValidateStatus()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	// meta := rpcClient.GetMeta(false)
	// log.Println(GreenTxt("Config Path:  "), config.GetPathToConfig(""))
	// log.Println(GreenTxt("Cache Path:   "), config.GetCachePath())
	// log.Println(GreenTxt("Index Path:   "), config.GetIndexPath())
	// log.Println(GreenTxt("Rpc Provider: "), config.ReadTrueBlocks().Settings.RpcProvider)
	// log.Printf("%s%d, %d (%d), %d (%d), %d (%d), %d (%d)\n", GreenTxt("Progress:      "),
	//	meta.Latest,
	//	meta.Finalized, (meta.Latest - meta.Finalized),
	//	meta.Staging, (meta.Staging - meta.Finalized),
	//	meta.Ripe, (meta.Ripe - meta.Staging),
	//	meta.Unripe, (meta.Unripe - meta.Ripe))
	// return nil
	return opts.Globals.PassItOn("cacheStatus", opts.ToCmdLine())
	// EXISTING_CODE
}

func ServeStatus(w http.ResponseWriter, r *http.Request) bool {
	opts := FromRequest(w, r)

	err := opts.ValidateStatus()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	// opts.Globals.PassItOn("cacheStatus", opts.ToCmdLine())
	return false
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE

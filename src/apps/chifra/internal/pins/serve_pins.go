package pinsPkg

import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/server/exec"
//	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
)

func ServePins(w http.ResponseWriter, r *http.Request) {
	err := pinlib.EstablishIndexFolders()
	if err != nil {
		return
	}

	opts := FromRequest(w, r)

	err = opts.ValidateOptionsPins()
	if err != nil {
		exec.RespondWithError(w, http.StatusInternalServerError, opts.Globals.TestMode, err)
		return
	}

	if opts.List {
		response, err := ListInternal(opts)
		if err != nil {
			exec.RespondWithError(w, http.StatusInternalServerError, opts.Globals.TestMode, err)
			return
		}
		exec.Respond(w, http.StatusOK, opts.Globals.Format, response)

	} else if opts.Init {
		err := InitInternal(opts)
		if err != nil {
			exec.RespondWithError(w, http.StatusInternalServerError, opts.Globals.TestMode, err)
		}
	}
}

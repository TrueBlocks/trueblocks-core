package pinsPkg

import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/server/exec"
)

func ServePins(w http.ResponseWriter, r *http.Request) {
	opts := FromRequest(w, r)

	err := pinlib.EstablishIndexFolders()
	if err != nil {
		return
	}

	err = opts.ValidatePins()
	if err != nil {
		exec.RespondWithError(w, http.StatusInternalServerError, &opts.Globals, err)
		return
	}

	if opts.List {
		err := opts.ListInternal()
		if err != nil {
			exec.RespondWithError(w, http.StatusInternalServerError, &opts.Globals, err)
			return
		}
	}

	if opts.Init {
		err := opts.InitInternal()
		if err != nil {
			exec.RespondWithError(w, http.StatusInternalServerError, &opts.Globals, err)
		}
	}
}

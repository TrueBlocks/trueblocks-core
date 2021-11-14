package pinsPkg

import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib"
)

func ServePins(w http.ResponseWriter, r *http.Request) {
	opts := FromRequest(w, r)

	err := pinlib.EstablishIndexFolders()
	if err != nil {
		return
	}

	err = opts.ValidatePins()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return
	}

	if opts.List {
		err := opts.ListInternal()
		if err != nil {
			opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
			return
		}
	}

	if opts.Init {
		err := opts.InitInternal()
		if err != nil {
			opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		}
	}
}

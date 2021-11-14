package abisPkg

import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/server/exec"
)

func ServeAbis(w http.ResponseWriter, r *http.Request) {
	opts := FromRequest(w, r)

	err := opts.ValidateAbis()
	if err != nil {
		exec.RespondWithError(w, http.StatusInternalServerError, &opts.Globals, err)
		return
	}

	if len(opts.Find) > 0 {
		err = opts.FindInternal()
		if err != nil {
			exec.RespondWithError(w, http.StatusInternalServerError, &opts.Globals, err)
			return
		}
		return
	}
}

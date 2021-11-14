package abisPkg

import (
	"net/http"
	"strings"
)

func ServeAbis(w http.ResponseWriter, r *http.Request) {
	opts := FromRequest(w, r)

	err := opts.ValidateAbis()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return
	}

	if len(opts.Find) > 0 {
		err = opts.FindInternal()
		if err != nil {
			opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
			return
		}
		return
	}

	Options.Globals.PassItOn("grabABI", opts.ToDashStr(), strings.Join(opts.Addrs, " "))
}

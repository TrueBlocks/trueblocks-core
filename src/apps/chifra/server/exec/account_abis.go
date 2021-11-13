package exec

import (
	"net/http"
	"os"

	abiPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/abis"
)

func ServeAbis(w http.ResponseWriter, r *http.Request) {
	opts := abiPkg.FromRequest(r)

	err := opts.ValidateOptionsAbis()
	if err != nil {
		RespondWithError(w, http.StatusInternalServerError, os.Getenv("TEST_MODE") == "true", err)
		return
	}

	results, err := abiPkg.FindInternal(opts)
	if err != nil {
		RespondWithError(w, http.StatusInternalServerError, os.Getenv("TEST_MODE") == "true", err)
		return
	}

	Respond(w, http.StatusOK, r.URL.Query().Get("fmt"), results)
}

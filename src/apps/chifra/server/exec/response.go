package exec

import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

var formatToMimeType = map[string]string{
	"api":  "application/json",
	"json": "application/json",
	"csv":  "text/csv",
	"txt":  "text/tab-separated-values",
}

// RespondWithError marshals the given error err into JSON
// that can be returned to the client and sets httpStatus HTTP error status code
func RespondWithError(w http.ResponseWriter, httpStatus int, opts *globals.GlobalOptionsType, err error) {
	marshalled, err := output.AsJsonBytes(&output.JsonFormatted{
		Errors: []string{err.Error()},
	}, opts)
	if err != nil {
		panic(err)
	}

	w.WriteHeader(httpStatus)
	w.Write(marshalled)
}

// Respond decides which format should be used, calls the right Responder, sets HTTP status code
// and writes a response
func Respond(w http.ResponseWriter, httpStatus int, opts *globals.GlobalOptionsType, responseData interface{}) {
	formatNotEmpty := opts.Format
	if formatNotEmpty == "" {
		formatNotEmpty = "api"
	}

	w.Header().Set("Content-Type", formatToMimeType[formatNotEmpty])
	err := output.Output(opts, w, formatNotEmpty, responseData)
	if err != nil {
		RespondWithError(w, http.StatusInternalServerError, opts, err)
	}
}

package serve

import (
	"net/http"

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
func RespondWithError(w http.ResponseWriter, httpStatus int, err error) {
	marshalled, err := output.AsJsonBytes(&output.JsonFormatted{
		Errors: []string{err.Error()},
	})
	if err != nil {
		panic(err)
	}

	w.WriteHeader(httpStatus)
	w.Write(marshalled)
}

// Respond decides which format should be used, calls the right Responder, sets HTTP status code
// and writes a response
func Respond(format string, w http.ResponseWriter, httpStatus int, responseData interface{}) {
	formatNotEmpty := format
	if formatNotEmpty == "" {
		formatNotEmpty = "api"
	}

	// TODO: this function should never use a global variable
	output.Format = formatNotEmpty

	w.Header().Set("Content-Type", formatToMimeType[formatNotEmpty])
	err := output.Output(w, formatNotEmpty, responseData)
	if err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

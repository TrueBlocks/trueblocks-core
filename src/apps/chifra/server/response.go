package serve

import (
	"fmt"
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

// Repsonder type represents a function that can be used to respond with a given format
type Responder func(w http.ResponseWriter, httpStatus int, responseData output.WithFormat)

// The map below maps format string to Responder
var formatToResponder = map[string]Responder{
	"api":  RespondWithJson,
	"json": RespondWithJson,
	"csv":  RespondWithCsv,
	"txt":  RespondWithTsv,
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

// RespondWithJson marshals data into JSON that can be returned
// to the client and sets the given HTTP status code
func RespondWithJson(w http.ResponseWriter, httpStatus int, responseData output.WithFormat) {
	marshalled, err := output.AsJsonBytes(&output.JsonFormatted{
		Data: responseData.GetJsonOutput(),
	})
	if err != nil {
		panic(err)
	}

	w.WriteHeader(httpStatus)
	w.Write(marshalled)
}

// RespondWithCsv turns data into CSV, sets the given HTTP status code, MIME type and writes
// a response
func RespondWithCsv(w http.ResponseWriter, httpStatus int, responseData output.WithFormat) {
	w.Header().Set("Content-Type", "text/csv")
	data := responseData.GetCsvOutput()
	result := output.AsCsv(data)

	w.Write([]byte(result))
}

// RespondWithTsv turns data into TSV, sets the given HTTP status code, MIME type and writes
// a response
func RespondWithTsv(w http.ResponseWriter, httpStatus int, responseData output.WithFormat) {
	w.Header().Set("Content-Type", "text/tab-separated-values")
	data := responseData.GetCsvOutput()
	result, err := output.AsTsv(data)
	if err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
		return
	}

	w.Write(result)
}

// Respond decides which format should be used, calls the right Responder, sets HTTP status code
// and writes a response
func Respond(format string, w http.ResponseWriter, httpStatus int, responseData output.WithFormat) {
	formatNotEmpty := format
	if formatNotEmpty == "" {
		formatNotEmpty = "api"
	}

	// TODO: this function should never use a global variable
	output.Format = formatNotEmpty

	responder := formatToResponder[formatNotEmpty]
	if responder == nil {
		// TODO: this error message is copied from a test
		RespondWithError(w, http.StatusBadRequest, fmt.Errorf("The --fmt option (%s) must be one of [ json | txt | csv | api ]", formatNotEmpty))
		return
	}

	responder(w, httpStatus, responseData)
}

package serve

import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

// RespondWithError marshals the given error err into JSON
// that can be returned to the client and sets httpStatus HTTP error status code
func RespondWithJsonError(w http.ResponseWriter, httpStatus int, err error) {
	RespondWithJson(w, httpStatus, &output.JsonFormatted{
		Errors: []string{
			err.Error(),
		},
	})
}

// RespondWithJsonSuccess marshals the given responseData into JSON that
// can be returned to the client and sets statusOK HTTP status code
func RespondWithJsonSuccess(w http.ResponseWriter, responseData interface{}) {
	RespondWithJson(w, http.StatusOK, &output.JsonFormatted{
		Data: responseData,
	})
}

// RespondWithJson marshals data into JSON that can be returned
// to the client and sets the given HTTP status code
func RespondWithJson(w http.ResponseWriter, httpStatus int, j *output.JsonFormatted) {
	marshalled, err := output.AsJsonBytes(j)
	if err != nil {
		panic(err)
	}

	w.WriteHeader(httpStatus)
	w.Write(marshalled)
}

package serve

import (
	"encoding/json"
	"net/http"
)

type ResponseBody map[string]interface{}

// RespondWithError marshals the given error err into JSON
// that can be returned to the client
func RespondWithError(w http.ResponseWriter, err error) {
	w.WriteHeader(http.StatusInternalServerError)
	jsonOutput, err := json.Marshal(ResponseBody{
		"errors": err.Error(),
	})
	if err != nil {
		panic(err)
	}
	w.Write(jsonOutput)
}

// RespondWithJson marshals data into JSON that can be returned
// to the client
func RespondWithJson(w http.ResponseWriter, data interface{}) {
	output, err := json.Marshal(ResponseBody{
		"data": data,
	})
	if err != nil {
		panic(err)
	}
	w.Write(output)
}

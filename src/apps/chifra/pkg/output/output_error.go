package output

import (
	"encoding/json"
	"net/http"
)

// RespondWithError marshals an err into JSON and returns the bytes
// back to the caller httpStatus HTTP error status code
func RespondWithError(w http.ResponseWriter, httpStatus int, err error) {
	type ErrorResponse struct {
		Errors []string `json:"errors,omitempty"`
	}
	marshalled, _ := json.MarshalIndent(ErrorResponse{Errors: []string{err.Error()}}, "", "  ")
	w.WriteHeader(httpStatus)
	w.Write(marshalled)
}

// RespondWithError marshals an err into JSON and returns the bytes
// back to the caller httpStatus HTTP error status code
func RespondWithErrorMsg(w http.ResponseWriter, httpStatus int, msg string, err error) {
	type ErrorResponse struct {
		Errors []string `json:"errors,omitempty"`
	}
	marshalled, _ := json.MarshalIndent(ErrorResponse{Errors: []string{msg + err.Error()}}, "", "  ")
	w.WriteHeader(httpStatus)
	w.Write(marshalled)
}

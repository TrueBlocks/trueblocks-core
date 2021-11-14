package servePkg

import (
	"net/http"
	"os"
)

// TODO: this should be soon part of pkg/utils
// var Off string = "\033[0m"
var Yellow string = "\033[33m"

// Runs initiates websocket pool and starts the server
func RunInternal(port string) error {
	os.Setenv("API_MODE", "true")

	// Start listening for web sockets
	RunWebsocketPool()

	// Start listening for requests
	return http.ListenAndServe(port, NewRouter())
}

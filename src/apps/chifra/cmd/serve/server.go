package serve

import "net/http"

// TODO: this should be soon part of pkg/utils
var Off string = "\033[0m"
var Yellow string = "\033[33m"

// Runs initiates websocket pool and starts the server
func Run(port string) error {
	// Start listening for web sockets
	RunWebsocketPool()

	// Start listening for requests
	return http.ListenAndServe(port, NewRouter())
}

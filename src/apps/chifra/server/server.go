package servePkg

import (
	"net/http"
	"os"
)

// Runs initiates websocket pool and starts the server
func RunInternal(port string) error {
	os.Setenv("API_MODE", "true")

	// Start listening for web sockets
	RunWebsocketPool()

	// Start listening for requests
	return http.ListenAndServe(port, NewRouter())
}

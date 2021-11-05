package serve

import "net/http"

// Runs initiates websocket pool and starts the server
func Run(port string) error {
	// Start listening on web sockets
	RunWebsocketPool()

	// Start listening for requests
	return http.ListenAndServe(port, NewRouter())
}

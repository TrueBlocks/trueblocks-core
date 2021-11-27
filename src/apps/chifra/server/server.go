// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

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

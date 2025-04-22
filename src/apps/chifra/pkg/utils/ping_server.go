package utils

import (
	"context"
	"net/http"
	"time"
)

// PingServer sends a GET request to the provided URL and returns true if
// the server responds with a 200 status code.
func PingServer(serverUrl string) bool {
	ctx, cancel := context.WithTimeout(context.Background(), 2*time.Second)
	defer cancel()

	req, err := http.NewRequestWithContext(ctx, "GET", serverUrl, nil)
	if err != nil {
		return false
	}

	clientHTTP := &http.Client{}
	resp, err := clientHTTP.Do(req)
	if err != nil {
		return false
	}
	defer resp.Body.Close()

	return resp.StatusCode == http.StatusOK
}

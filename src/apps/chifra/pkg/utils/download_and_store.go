package utils

import (
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"os"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/file"
)

// DownloadAndStoreJSON is a generic function that:
//   - Downloads from the given URL if the local file is stale.
//   - Stores it in the given file path.
//   - Unmarshals the JSON bytes into a type T and returns a *T.
//
// T must be a Go type compatible with the JSON structure (e.g. a struct or slice).
func DownloadAndStoreJSON[T any](url, filename string, cacheTTL time.Duration) (*T, error) {
	// Use your existing caching logic from "downloadAndStore"
	bytes, err := downloadAndStore(url, filename, cacheTTL)
	if err != nil {
		var zero T
		return &zero, err
	}

	var result T
	if err := json.Unmarshal(bytes, &result); err != nil {
		return &result, err
	}
	return &result, nil
}

// downloadAndStore retrieves data from the specified URL and caches it in the provided
// filename for up to `dur`. If the file already exists and is newer than `dur`, it returns
// the file's contents without making a network request. Otherwise, it fetches from the URL.
//
// If the server returns 404, the function writes an empty file to disk and returns a zero-length
// byte slice. For other non-200 status codes, it returns an error.
//
// If the response is valid JSON, it is pretty-formatted before being saved; otherwise it is
// saved as-is. The function returns the written file content as a byte slice.
func downloadAndStore(url, filename string, dur time.Duration) ([]byte, error) {
	if file.FileExists(filename) {
		lastModDate, err := file.GetModTime(filename)
		if err != nil {
			return nil, err
		}
		if time.Since(lastModDate) < dur {
			data, err := os.ReadFile(filename)
			if err != nil {
				return nil, err
			}
			return data, nil
		}
	}

	resp, err := http.Get(url)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	if resp.StatusCode == http.StatusNotFound {
		// If the file doesn't exist remotely, store an empty file
		if err := os.WriteFile(filename, []byte{}, 0644); err != nil {
			return nil, err
		}
		// Optionally update its mod time
		_ = file.Touch(filename)
		return []byte{}, nil
	} else if resp.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("received status %d %s for URL %s",
			resp.StatusCode, resp.Status, url)
	}

	rawData, err := io.ReadAll(resp.Body)
	if err != nil {
		return nil, err
	}

	var prettyData []byte
	if json.Valid(rawData) {
		var jsonData interface{}
		if err := json.Unmarshal(rawData, &jsonData); err != nil {
			return nil, err
		}
		prettyData, err = json.MarshalIndent(jsonData, "", "  ")
		if err != nil {
			return nil, err
		}
	} else {
		prettyData = rawData
	}

	if err := os.WriteFile(filename, prettyData, 0644); err != nil {
		return nil, err
	}

	_ = file.Touch(filename)

	return prettyData, nil
}

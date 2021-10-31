package manifest

import (
	"encoding/json"
	"io"
)

// ReadJSONManifest reads manifest in JSON format
func ReadJSONManifest(reader io.Reader) (*Manifest, error) {
	decoder := json.NewDecoder(reader)
	manifest := &Manifest{}

	err := decoder.Decode(manifest)

	return manifest, err
}

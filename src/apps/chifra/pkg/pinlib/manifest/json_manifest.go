package manifest

import (
	"encoding/json"
	"io"
)

func ReadJSONManifest(reader io.Reader) (*Manifest, error) {
	decoder := json.NewDecoder(reader)
	manifest := &Manifest{}

	err := decoder.Decode(manifest)

	return manifest, err
}

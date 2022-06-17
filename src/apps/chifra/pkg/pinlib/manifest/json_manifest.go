// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

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

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package manifest

import (
	"encoding/json"
	"strings"
	"testing"
)

var manifestSource = `
{
  "version": "2",
  "chain": "mainnet",
  "schemas": "QmUou7zX2g2tY58LP1A2GyP5RF9nbJsoxKTp299ah3svgb",
  "chunks": [
    {
      "fileName": "000000000-000000000",
      "bloomHash": "QmPQEgUm7nzQuW9HYyWp5Ff3aoUwg2rsxDngyuyddJTvrv",
      "indexHash": "QmZ5Atm8Z7aFLz2EycK4pVMHuH4y3PDGspuFejnE9fx2i5"
    },
    {
      "fileName": "000000001-000350277",
      "bloomHash": "QmZgrWAJLidkHJRLDVoZGCWAgmmcQEDCDM65XL5ZbAXxCM",
      "indexHash": "QmP1KvDPUJ1MqsCYcicJgTf5sxN7WjT7dZsrfBk2Jg3mSe"
    },
    {
      "fileName": "000350278-000590502",
      "bloomHash": "QmcU9tUUzyMLsBpKTpbYg2Xp6W5mmEqEDH3RSZzzcqPzMB",
      "indexHash": "QmavFBhUeRBnTCcAFUeSstGLuba4neYaSNvaPurCuYfU1N"
    },
    {
      "fileName": "000590503-000749875",
      "bloomHash": "QmWE3MaqAuoLmzg5wu5yXfLa7jD1HVjsz9898U4D2iN6i3",
      "indexHash": "Qmd3xF6eF9UxkWmru6i64GRy2L7iqxvAxYG9aZKZLEqRdN"
    },
    {
      "fileName": "000749876-000864336",
      "bloomHash": "QmXW2AQoZRV8qMw6DCn1vJEdz37wH1Q9WTjgecR4JHj5xH",
      "indexHash": "QmRedJaRUQEZoP3v32EYXGAiYT9gvXSGwtQMUcdRRJ7GQW"
    }
   ]
}
`

func TestReadManifest(t *testing.T) {
	m := &Manifest{}
	err := json.NewDecoder(strings.NewReader(manifestSource)).Decode(m)
	if err != nil {
		t.Error(err)
	}

	type TestCases struct {
		name     string
		field    string
		expected string
	}

	cases := []TestCases{
		{
			name:     "Version",
			field:    m.Version,
			expected: "2",
		},
		{
			name:     "Chain",
			field:    m.Chain,
			expected: "mainnet",
		},
		{
			name:     "Schemas",
			field:    m.Schemas.String(),
			expected: "QmUou7zX2g2tY58LP1A2GyP5RF9nbJsoxKTp299ah3svgb",
		},
	}

	for _, tc := range cases {
		if tc.field != tc.expected {
			t.Errorf("Wrong %s: %s", tc.name, tc.field)
		}
	}

	newPins := m.Chunks

	if len(newPins) != 5 {
		t.Errorf("Incorrect NewPins length: %d", len(newPins))
	}

	if newPins[0].BloomHash != "QmPQEgUm7nzQuW9HYyWp5Ff3aoUwg2rsxDngyuyddJTvrv" {
		t.Errorf("Wrong NewPins[0].BloomHash: %s", newPins[0].BloomHash)
	}
}

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package manifest

import (
	"strings"
	"testing"
)

var manifestSource = `
{
  "version": "2",
  "chain": "mainnet",
  "chainId": 1,
  "indexFormat": "Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2",
  "bloomFormat": "QmNhPk39DUFoEdhUmtGARqiFECUHeghyeryxZM9kyRxzHD",
  "commitHash": "f29699d3281e41cb011ddfbe50b7f01bfe5e3c53",
  "names": "QmP4i6ihnVrj8Tx7cTFw4aY6ungpaPYxDJEZ7Vg1RSNSdm",
  "timestamps": "QmcvjroTiE95LWeiP8HHq1YA3ysRchLuVx8HLQui8WcSBV",
  "blockRange": "000000000-000864336",
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
	m, err := ReadJSONManifest(strings.NewReader(manifestSource))
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
			field:    m.Version.String(),
			expected: "2",
		},
		{
			name:     "Chain",
			field:    m.Chain,
			expected: "mainnet",
		},
		{
			name:     "ChainId",
			field:    m.ChainId.String(),
			expected: "1",
		},
		{
			name:     "IndexFormat",
			field:    m.IndexFormat,
			expected: "Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2",
		},
		{
			name:     "BloomFormat",
			field:    m.BloomFormat,
			expected: "QmNhPk39DUFoEdhUmtGARqiFECUHeghyeryxZM9kyRxzHD",
		},
		{
			name:     "CommitHash",
			field:    m.CommitHash,
			expected: "f29699d3281e41cb011ddfbe50b7f01bfe5e3c53",
		},
		{
			name:     "Names",
			field:    m.Names,
			expected: "QmP4i6ihnVrj8Tx7cTFw4aY6ungpaPYxDJEZ7Vg1RSNSdm",
		},
		{
			name:     "Timestamps",
			field:    m.Timestamps,
			expected: "QmcvjroTiE95LWeiP8HHq1YA3ysRchLuVx8HLQui8WcSBV",
		},
	}

	for _, tc := range cases {
		if tc.field != tc.expected {
			t.Errorf("Wrong %s: %s", tc.name, tc.field)
		}
	}

	if m.BlockRange[0] != 0 {
		t.Errorf("Wrong NewBlockRange[0]: %d", m.BlockRange[0])
	}

	if m.BlockRange[1] != 864336 {
		t.Errorf("Wrong NewBlockRange[1]: %d", m.BlockRange[1])
	}

	newPins := m.Chunks

	if len(newPins) != 5 {
		t.Errorf("Incorrect NewPins length: %d", len(newPins))
	}

	if newPins[0].BloomHash != "QmPQEgUm7nzQuW9HYyWp5Ff3aoUwg2rsxDngyuyddJTvrv" {
		t.Errorf("Wrong NewPins[0].BloomHash: %s", newPins[0].BloomHash)
	}
}

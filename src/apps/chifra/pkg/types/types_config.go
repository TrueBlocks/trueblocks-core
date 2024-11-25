// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
package types

import (
	"encoding/json"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/configtypes"
)

type Config struct {
	configtypes.Config
}

func (s Config) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Config) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	// keys := map[string]any{}
	// for key, value := range s.Keys {
	// 	keys[key] = value
	// }
	c := map[string]any{}
	for key, value := range s.Chains {
		c[key] = value
	}
	return Model{
		Data: map[string]any{
			"version":   s.Version,
			"settings":  s.Settings,
			"keys":      s.Keys,
			"pinnging":  s.Pinning,
			"unchained": s.Unchained,
			"chains":    s.Chains,
		},
		Order: []string{},
	}
}

func (s *Config) FinishUnmarshal() {
}

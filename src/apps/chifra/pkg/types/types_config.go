// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
package types

import (
	"encoding/json"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/configtypes"
)

type Config struct {
	Version   configtypes.VersionGroup          `json:"version" toml:"version"`
	Settings  configtypes.SettingsGroup         `json:"settings" toml:"settings"`
	Keys      map[string]configtypes.KeyGroup   `json:"keys" toml:"keys"`
	Pinning   configtypes.PinningGroup          `json:"pinning" toml:"pinning"`
	Unchained configtypes.UnchainedGroup        `json:"unchained" toml:"unchained,omitempty" comment:"Do not edit these values unless instructed to do so."`
	Chains    map[string]configtypes.ChainGroup `json:"chains" toml:"chains"`
}

func (s Config) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Config) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	_ = extraOpts
	_ = format
	_ = verbose
	_ = chain
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

func (s *Config) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
}

func (s *Config) ShallowCopy() Config {
	return *s
}

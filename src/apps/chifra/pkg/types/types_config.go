// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
package types

import (
	"encoding/json"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/configtypes"
)

type Config struct {
	Version   configtypes.VersionGroup          `json:"version" toml:"version"`
	Settings  configtypes.SettingsGroup         `json:"settings" toml:"settings"`
	Keys      map[string]configtypes.KeyGroup   `json:"keys" toml:"keys"`
	Pinning   configtypes.PinningGroup          `json:"pinning" toml:"pinning"`
	Unchained configtypes.UnchainedGroup        `json:"unchained" toml:"unchained,omitempty" comment:"Do not edit these values unless instructed to do so."`
	Chains    map[string]configtypes.ChainGroup `json:"chains" toml:"chains"`
	Calcs     *ConfigCalcs                      `json:"calcs,omitempty"`
}

func (s Config) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Config) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"version",
		"settings",
		"keys",
		"pinning",
		"unchained",
		"chains",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this Config.
func (s *Config) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		"version":   s.Version,
		"settings":  s.Settings,
		"keys":      s.Keys,
		"pinning":   s.Pinning,
		"unchained": s.Unchained,
		"chains":    s.Chains,
	}

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this Config.
func (s *Config) CalcMap(p *ModelProps) map[string]any {
	_ = p // delint
	model := map[string]any{}

	return model
}

func (s *Config) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// ConfigCalcs holds lazy-loaded calculated fields for Config
type ConfigCalcs struct {
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *Config) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
	_ = fieldFilter // delint
	if s.Calcs != nil {
		return nil
	}

	calcMap := s.CalcMap(p)
	if len(calcMap) == 0 {
		return nil
	}

	jsonBytes, err := json.Marshal(calcMap)
	if err != nil {
		return err
	}

	s.Calcs = &ConfigCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

func (s *Config) ShallowCopy() Config {
	return *s
}

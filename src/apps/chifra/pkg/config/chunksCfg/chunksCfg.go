// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksCfg

import (
	"os"
	"path/filepath"
	"reflect"
	"strconv"
	"strings"

	"github.com/BurntSushi/toml"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type ChunksSettings struct {
	Pinata_api_key        string  `json:"pinataApiKey,omitempty"`
	Pinata_secret_api_key string  `json:"pinataSecretApiKey,omitempty"`
	Pinata_jwt            string  `json:"pinataJwt,omitempty"`
	Estuary_key           string  `json:"estuaryKey,omitempty"`
	Sleep                 float64 `json:"sleep"`
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultSettings = ChunksSettings{
	Pinata_api_key:        "",
	Pinata_secret_api_key: "",
	Pinata_jwt:            "",
	Estuary_key:           "",
	Sleep:                 float64(utils.NOPOS),
	// EXISTING_CODE
	// EXISTING_CODE
}

var Unset = ChunksSettings{
	Pinata_api_key:        "",
	Pinata_secret_api_key: "",
	Pinata_jwt:            "",
	Estuary_key:           "",
	Sleep:                 0.0,
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *ChunksSettings) isDefault(chain, fldName string) bool {
	def := GetDefault(chain)
	switch fldName {
	case "Pinata_api_key":
		return s.Pinata_api_key == def.Pinata_api_key
	case "Pinata_secret_api_key":
		return s.Pinata_secret_api_key == def.Pinata_secret_api_key
	case "Pinata_jwt":
		return s.Pinata_jwt == def.Pinata_jwt
	case "Estuary_key":
		return s.Estuary_key == def.Estuary_key
	case "Sleep":
		return s.Sleep == def.Sleep
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return false
}

func (s *ChunksSettings) TestLog(chain string, test bool) {
	if !test {
		logger.TestLog(!s.isDefault(chain, "Pinata_api_key"), "Pinata_api_key: ", s.Pinata_api_key)
		logger.TestLog(!s.isDefault(chain, "Pinata_secret_api_key"), "Pinata_secret_api_key: ", s.Pinata_secret_api_key)
		logger.TestLog(!s.isDefault(chain, "Pinata_jwt"), "Pinata_jwt: ", s.Pinata_jwt)
		logger.TestLog(!s.isDefault(chain, "Estuary_key"), "Estuary_key: ", s.Estuary_key)
		logger.TestLog(!s.isDefault(chain, "Sleep"), "Sleep: ", s.Sleep)
	}
	// EXISTING_CODE
	// EXISTING_CODE
}

func GetDefault(chain string) ChunksSettings {
	ret := defaultSettings
	// EXISTING_CODE
	// EXISTING_CODE
	return ret
}

const configFilename = "trueBlocks.toml"

// GetSettings retrieves config from (in order) default, config, environment, optionally provided cmdLine
func GetSettings(chain string, cmdLine *ChunksSettings) (ChunksSettings, error) {
	type TomlFile struct {
		Settings ChunksSettings
	}

	// Start with the defalt values...
	ret := GetDefault(chain)

	tt := reflect.TypeOf(defaultSettings)
	fieldList, _, _ := utils.GetFields(&tt, "txt", true)

	configFn := filepath.Join(config.GetPathToChainConfig(chain), configFilename)
	if strings.Contains(configFn, "trueBlocks.toml") {
		configFn = filepath.Join(config.GetPathToRootConfig(), configFilename)
	}

	if file.FileExists(configFn) {
		var t TomlFile
		// ...pick up values from toml file...
		if _, err := toml.Decode(utils.AsciiFileToString(configFn), &t); err != nil {
			return ChunksSettings{}, err
		}
		ret.overlay(chain, t.Settings)
	}

	// ...check the environment...
	for _, field := range fieldList {
		envKey := toEnvStr(field)
		envValue := os.Getenv(envKey)
		if envValue != "" {
			fName := utils.MakeFirstUpperCase(field)
			fld := reflect.ValueOf(&ret).Elem().FieldByName(fName)
			if fld.Kind() == reflect.String {
				fld.SetString(envValue)
			} else if fld.Kind() == reflect.Bool {
				if envValue == "true" {
					fld.SetBool(true)
				}
			} else {
				if v, err := strconv.ParseUint(envValue, 10, 64); err == nil {
					fld.SetUint(v)
				}
			}
		}
	}

	if cmdLine != nil {
		ret.overlay(chain, *cmdLine)
	}

	return ret, nil
}

func (base *ChunksSettings) overlay(chain string, overlay ChunksSettings) {
	if !overlay.isDefault(chain, "Pinata_api_key") && overlay.Pinata_api_key != "" {
		base.Pinata_api_key = overlay.Pinata_api_key
	}
	if !overlay.isDefault(chain, "Pinata_secret_api_key") && overlay.Pinata_secret_api_key != "" {
		base.Pinata_secret_api_key = overlay.Pinata_secret_api_key
	}
	if !overlay.isDefault(chain, "Pinata_jwt") && overlay.Pinata_jwt != "" {
		base.Pinata_jwt = overlay.Pinata_jwt
	}
	if !overlay.isDefault(chain, "Estuary_key") && overlay.Estuary_key != "" {
		base.Estuary_key = overlay.Estuary_key
	}
	if !overlay.isDefault(chain, "Sleep") && overlay.Sleep != 0.0 {
		base.Sleep = overlay.Sleep
	}
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
func PinningKeys(chain string) (string, string, string) {
	s, _ := GetSettings(chain, nil)
	return s.Pinata_api_key, s.Pinata_secret_api_key, s.Estuary_key
}

func toEnvStr(name string) string {
	return "TB_SETTINGS_" + strings.ToUpper(strings.Replace(name, "_", "", -1))
}

// EXISTING_CODE

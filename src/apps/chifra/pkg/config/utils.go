// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"os/user"
	"path"
	"runtime"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/spf13/viper"
)

var OsToPath = map[string]string{
	"linux":  ".local/share/trueblocks",
	"darwin": "Library/Application Support/TrueBlocks",
}

// GetConfigPath returns the path to the directory where the configuration files are
func GetConfigPath(fileName string) string {
	// These values are checked in CheckMigrations and will not proceed if not valid
	userOs := runtime.GOOS
	user, _ := user.Current()
	return path.Join(user.HomeDir, OsToPath[userOs], fileName)
}

// MustReadConfig calls v's ReadInConfig and fills values in the
// given targetStruct. Any error will result in a call to logger.Fatal
func MustReadConfig(v *viper.Viper, targetStruct interface{}, fileRequired bool) {
	v.AddConfigPath(GetConfigPath(""))
	v.SetEnvPrefix("TB")
	v.AutomaticEnv()
	v.SetEnvKeyReplacer(strings.NewReplacer(".", "_"))

	err := v.ReadInConfig()
	if err != nil {
		_, ok := err.(viper.ConfigFileNotFoundError)
		// We only require some files to be present
		if !ok || (ok && fileRequired) {
			logger.Fatal(err)
		}
	}

	err = v.Unmarshal(targetStruct)
	if err != nil {
		logger.Fatal(err)
	}
}

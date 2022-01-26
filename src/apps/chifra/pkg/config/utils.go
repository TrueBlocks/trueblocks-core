// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/spf13/viper"
)

// MustReadConfig calls Viper's ReadInConfig and fills values in the
// given targetStruct. Any error will result in a call to logger.Fatal
func MustReadConfig(v *viper.Viper, targetStruct interface{}, path string) {
	v.AddConfigPath(path)
	v.SetEnvPrefix("TB")
	v.AutomaticEnv()
	v.SetEnvKeyReplacer(strings.NewReplacer(".", "_"))

	err := v.ReadInConfig()
	if err != nil {
		_, ok := err.(viper.ConfigFileNotFoundError)
		// We only require some files to be present
		if !ok {
			logger.Fatal(err)
		}
	}

	err = v.Unmarshal(targetStruct)
	if err != nil {
		logger.Fatal(err)
	}
}

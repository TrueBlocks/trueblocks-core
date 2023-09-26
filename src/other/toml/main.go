package main

import (
	"log"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

var ff = "./testdata/trueBlocks.toml"
var out = "./testdata/output.toml"
var out1 = "./testdata/output1.toml"

func main() {
	var cfg config.ConfigFile
	if err := config.ReadConfigFile(ff, &cfg); err == nil {
		config.WriteConfigFile(out, &cfg)
		var cfg2 config.ConfigFile
		if err := config.ReadConfigFile(out, &cfg2); err == nil {
			config.WriteConfigFile(out1, &cfg2)
		} else {
			log.Fatal(err)
		}
	} else {
		log.Fatal(err)
	}
}

package main

import (
	"log"
	"os"

	"github.com/BurntSushi/toml"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

var ff = "./testdata/trueBlocks.toml"
var out = "./testdata/output.toml"
var out1 = "./testdata/output1.toml"

func main() {
	// Read it once...as the original
	var config2 config.ConfigFile
	var config config.ConfigFile
	if _, err := toml.DecodeFile(ff, &config); err == nil {
		// Write it back out as expected
		WriteToml(out, config)
		// Read it once...from the new format
		if _, err := toml.DecodeFile(out, &config2); err == nil {
			// Write it back out as expected
			WriteToml(out1, config2)
		} else {
			log.Fatal(err)
		}
	} else {
		log.Fatal(err)
	}
}

func WriteToml(fn string, config interface{}) error {
	if f, err := os.Create(fn); err != nil {
		log.Fatal(err)
	} else {
		defer f.Close()
		if err := toml.NewEncoder(f).Encode(config); err != nil {
			log.Fatal(err)
		}
		log.Println("Wrote", fn)
	}
	return nil
}

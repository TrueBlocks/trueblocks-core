package config

import (
	"github.com/BurntSushi/toml"
)

// readFile reads the toml config file into the given struct
func readFile(inFile string, contents interface{}) error {
	_, err := toml.DecodeFile(inFile, contents)
	return err
}

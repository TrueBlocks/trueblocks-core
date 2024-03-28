package config

import (
	"github.com/BurntSushi/toml"
)

// ReadToml reads the toml config file into the given struct
func ReadToml(inFile string, contents interface{}) error {
	_, err := toml.DecodeFile(inFile, contents)
	return err
}

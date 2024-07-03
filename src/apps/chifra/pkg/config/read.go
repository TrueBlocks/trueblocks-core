package config

import (
	"os"

	"github.com/pelletier/go-toml/v2"
)

// ReadToml reads the toml config file into the given struct
func ReadToml(inFile string, destination interface{}) error {
	bytes, err := os.ReadFile(inFile)
	if err != nil {
		return err
	}

	return toml.Unmarshal(bytes, destination)
}

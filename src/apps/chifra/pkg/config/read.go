package config

import (
	"github.com/knadh/koanf/v2"
)

// ReadToml reads the toml config file into the given struct
func ReadToml(inFile string, contents interface{}) error {
	config := koanf.New(".")
	if err := loadFromTomlFile(config, inFile); err != nil {
		return err
	}
	return config.Unmarshal("", contents)
}

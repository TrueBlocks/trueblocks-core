package types

import (
	"errors"
	"fmt"
	"io/fs"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

var (
	ErrTomlFilesCheck       = errors.New("error checking for .toml files")
	ErrRequirementsNotMet   = errors.New("generation requirements not met: either cmd-line-options.csv must exist or classDefinitions folder must exist with at least one .toml file")
	ErrEmptyTemplatesFolder = errors.New("templates folder exists but is empty; it must contain cmd-line-options.csv and/or classDefinitions/ folder with .toml files")
)

func (cb *CodeBase) isValidSetup() error {
	templatesPath := GetTemplatePath()

	// Check if templates folder exists but is empty
	isEmpty := true
	files, err := filepath.Glob(filepath.Join(templatesPath, "*"))
	if err == nil && len(files) > 0 {
		isEmpty = false
	}

	if isEmpty {
		return ErrEmptyTemplatesFolder
	}

	cmdLineOptionsExists := file.FileExists(filepath.Join(templatesPath, "cmd-line-options.csv"))
	classDefPath := filepath.Join(templatesPath, "classDefinitions")
	classDefFolderExists := file.FolderExists(classDefPath)

	hasTomlFiles := false
	if classDefFolderExists {
		err := filepath.Walk(classDefPath, func(path string, info fs.FileInfo, err error) error {
			if err != nil {
				return err
			}
			if !info.IsDir() && strings.HasSuffix(path, ".toml") {
				hasTomlFiles = true
				return filepath.SkipAll
			}
			return nil
		})
		if err != nil {
			return fmt.Errorf("%w: %s", ErrTomlFilesCheck, err)
		}
	}

	if cmdLineOptionsExists {
		return nil
	}

	if classDefFolderExists && hasTomlFiles {
		return nil
	}

	return ErrRequirementsNotMet
}

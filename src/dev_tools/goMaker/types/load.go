package types

import (
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// LoadDefinitions loads the definitions from the data-models folder
func LoadDefinitions() (CodeBase, error) {
	cwd, _ := os.Getwd()
	if !strings.HasSuffix(strings.Trim(cwd, "/"), "trueblocks-core") {
		return CodeBase{}, fmt.Errorf("this program must be run from the ./trueblocks-core folder")
	}

	thePath := "src/other/data-models/"
	if !file.FolderExists(thePath) {
		return CodeBase{}, fmt.Errorf("the path %s does not exist", thePath)
	}

	codeBase, err := LoadCodebase(thePath)
	if err != nil {
		return CodeBase{}, err
	}

	if len(codeBase.Commands) == 0 {
		return CodeBase{}, fmt.Errorf("no commands were found in %s", thePath)
	}

	if len(codeBase.Structures) == 0 {
		return CodeBase{}, fmt.Errorf("no structures were found in %s", thePath)
	}

	return codeBase, nil
}

// LoadCodebase loads the two csv files and returns the codebase which
// contains all the commands (each with its own options and endpoint).
// This will also eventually carry the data types.
func LoadCodebase(thePath string) (CodeBase, error) {
	theMap := make(map[string]Command)

	options, err := LoadCsv[CmdLineOption, any](thePath+"cmd-line-options.csv", readCmdOption, nil)
	if err != nil {
		return CodeBase{}, err
	}

	for _, opt := range options {
		lower := strings.ToLower(opt.ApiRoute)
		if lower == "" {
			lower = strings.ToLower(opt.Group)
		}
		cmd := Command{
			Route:   opt.ApiRoute,
			Options: append(theMap[lower].Options, opt),
		}
		theMap[lower] = cmd
	}

	endpoints, err := LoadCsv[CmdLineEndpoint, any](thePath+"cmd-line-endpoints.csv", readCmdEndpoint, nil)
	if err != nil {
		return CodeBase{}, err
	}

	for _, endpoint := range endpoints {
		lower := strings.ToLower(endpoint.ApiRoute)
		if lower == "" {
			lower = strings.ToLower(endpoint.Group)
		}
		cmd := Command{
			Route:       endpoint.ApiRoute,
			Group:       endpoint.Group,
			Description: endpoint.Description,
			Options:     theMap[lower].Options,
			Endpoint:    endpoint,
		}
		theMap[lower] = cmd
	}

	var cb CodeBase
	cb.Structures = make(map[string]Structure)
	cb.Commands = make([]Command, 0, len(theMap))
	for _, cmd := range theMap {
		cmd.clean()
		cb.Commands = append(cb.Commands, cmd)
	}
	sort.Slice(cb.Commands, func(i, j int) bool {
		if cb.Commands[i].Route == cb.Commands[j].Route {
			return cb.Commands[i].Group < cb.Commands[j].Group
		}
		return cb.Commands[i].Route < cb.Commands[j].Route
	})

	thePath += "classDefinitions/"
	if err := ReadStructures(thePath, &cb); err != nil {
		return cb, err
	}

	if err := ReadMembers(thePath, &cb); err != nil {
		return cb, err
	}

	if true {
		fmt.Println(cb.String())
	}

	return cb, nil
}

func ReadStructures(thePath string, cb *CodeBase) error {
	if err := filepath.Walk(thePath, func(path string, info fs.FileInfo, err error) error {
		if err != nil {
			return err
		}
		if !info.IsDir() {
			if strings.HasSuffix(path, ".toml") {
				class := strings.TrimSuffix(filepath.Base(path), ".toml")
				type S struct {
					Settings Structure `toml:"settings"`
				}
				var f S
				err := config.ReadToml(path, &f)
				if err != nil {
					return err
				}
				if f.Settings.Class[0] == 'C' {
					f.Settings.Class = f.Settings.Class[1:]
				}
				f.Settings.Name = class
				if f.Settings.GoOutput == "documentation only" {
					f.Settings.GoOutput = "<docs_only>"
					f.Settings.DisableGo = true
				}
				cb.Structures[class] = f.Settings
			}
		}
		return nil
	}); err != nil {
		return err
	}
	return nil
}

func ReadMembers(thePath string, cb *CodeBase) error {
	if err := filepath.Walk(thePath, func(path string, info fs.FileInfo, err error) error {
		if err != nil {
			return err
		}
		if !info.IsDir() {
			if strings.HasSuffix(path, ".csv") {
				class := strings.TrimSuffix(filepath.Base(path), ".csv")
				structure := cb.Structures[class]
				var err error
				structure.Members, err = LoadCsv[Member, any](path, readMember, nil)
				if err != nil {
					return err
				}
				for i := 0; i < len(structure.Members); i++ {
					structure.Members[i].Num = (i + 1)
				}
				// sort.Slice(structure.Members, func(i, j int) bool {
				// 	if structure.Members[i].DocOrder != 0 && (structure.Members[i].DocOrder != structure.Members[j].DocOrder) {
				// 		return structure.Members[i].DocOrder < structure.Members[j].DocOrder
				// 	}
				// 	return structure.Members[i].Num < structure.Members[j].Num
				// })
				cb.Structures[class] = structure
				for i := 0; i < len(structure.Members); i++ {
					structure.Members[i].Class = cb.Structures[class].Class
				}
			}
		}
		return nil
	}); err != nil {
		return err
	}
	return nil
}

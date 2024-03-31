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
	var cb CodeBase
	options, err := LoadCsv[CmdLineOption, any](thePath+"cmd-line-options.csv", readCmdOption, nil)
	if err != nil {
		return CodeBase{}, err
	}

	endpoints, err := LoadCsv[CmdLineEndpoint, any](thePath+"cmd-line-endpoints.csv", readCmdEndpoint, nil)
	if err != nil {
		return CodeBase{}, err
	}

	structMap := make(map[string]Structure)
	err = cb.LoadStructures(thePath+"classDefinitions/", structMap)
	if err != nil {
		return cb, err
	}

	err = cb.LoadMembers(thePath+"classDefinitions/", structMap)
	if err != nil {
		return cb, err
	}

	cb.FinishLoad(options, endpoints, structMap)

	return cb, nil
}

func (cb *CodeBase) LoadStructures(thePath string, structMap map[string]Structure) error {
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
				structMap[class] = f.Settings
			}
		}
		return nil
	}); err != nil {
		return err
	}

	return nil
}

func (cb *CodeBase) LoadMembers(thePath string, structMap map[string]Structure) error {
	if err := filepath.Walk(thePath, func(path string, info fs.FileInfo, err error) error {
		if err != nil {
			return err
		}
		if !info.IsDir() {
			if strings.HasSuffix(path, ".csv") {
				class := strings.TrimSuffix(filepath.Base(path), ".csv")
				structure := structMap[class]
				var err error
				structure.Members, err = LoadCsv[Member, any](path, readMember, nil)
				if err != nil {
					return err
				}
				for i := 0; i < len(structure.Members); i++ {
					structure.Members[i].Num = (i + 1)
				}
				sort.Slice(structure.Members, func(i, j int) bool {
					if structure.Members[i].DocOrder != 0 && (structure.Members[i].DocOrder != structure.Members[j].DocOrder) {
						return structure.Members[i].DocOrder < structure.Members[j].DocOrder
					}
					return structure.Members[i].Num < structure.Members[j].Num
				})
				structMap[class] = structure
				for i := 0; i < len(structure.Members); i++ {
					structure.Members[i].Class = structMap[class].Class
				}
			}
		}
		return nil
	}); err != nil {
		return err
	}

	return nil
}

func (cb *CodeBase) FinishLoad(options []CmdLineOption, endpoints []CmdLineEndpoint, structMap map[string]Structure) {
	theMap := make(map[string]Command)

	producesMap := make(map[string][]string)

	// Create the structure array (and sort it by DocRoute) from the map
	cb.Structures = make([]Structure, 0, len(structMap))
	for _, st := range structMap {
		cb.Structures = append(cb.Structures, st)
		producers := strings.Split(strings.Replace(st.DocProducer, " ", "", -1), ",")
		for _, producer := range producers {
			producesMap[producer] = append(producesMap[producer], st.Class)
		}
		if cb.TypeToGroup == nil {
			cb.TypeToGroup = make(map[string]string)
		}
		dg := strings.Split(st.DocGroup, "-")
		if len(dg) > 1 {
			ddg := strings.ToLower(dg[1])
			ddg = strings.Replace(ddg, " ", "", -1)
			cb.TypeToGroup[strings.ToLower(st.Name)] = ddg
		}
	}
	sort.Slice(cb.Structures, func(i, j int) bool {
		return cb.Structures[i].DocRoute < cb.Structures[j].DocRoute
	})

	// Attach each option to its command (i.e. its route)
	for _, opt := range options {
		route := strings.ToLower(opt.ApiRoute)
		cmd := Command{
			Options: append(theMap[route].Options, opt),
		}
		theMap[route] = cmd
	}

	// Enhance the commands with information from the endpoints data (could have been combined,
	// but this way we can keep the data separate and still have a single command object)
	for _, endpoint := range endpoints {
		route := strings.ToLower(endpoint.ApiRoute)
		if route == "" {
			route = strings.ToLower(endpoint.Group)
		}
		sort.Slice(producesMap[route], func(i, j int) bool {
			return producesMap[route][i] < producesMap[route][j]
		})
		cmd := Command{
			Route:       endpoint.ApiRoute,
			Group:       endpoint.Group,
			Description: endpoint.Description,
			Options:     theMap[route].Options,
			Endpoint:    endpoint,
			Produces:    producesMap[route],
			cb:          cb,
		}
		theMap[route] = cmd
	}

	// Create a command array from the map and sort it by route within group
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

	if os.Getenv("TB_MAKER_DEBUG") == "true" {
		fmt.Println(cb.String())
	}
}

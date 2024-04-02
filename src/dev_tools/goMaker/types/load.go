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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// LoadDefinitions loads the definitions from the data-models folder
func LoadDefinitions() (CodeBase, error) {
	cwd, _ := os.Getwd()
	if !strings.HasSuffix(strings.Trim(cwd, "/"), "trueblocks-core") {
		return CodeBase{}, fmt.Errorf("this program must be run from the ./trueblocks-core folder")
	}

	thePath := "src/dev_tools/goMaker/templates/"
	if !file.FolderExists(thePath) {
		return CodeBase{}, fmt.Errorf("the path %s does not exist", thePath)
	}

	codeBase, err := LoadCodebase(thePath)
	if err != nil {
		return CodeBase{}, err
	}

	return codeBase, nil
}

// LoadCodebase loads the two csv files and returns the codebase which
// contains all the commands (each with its own options and endpoint).
// This will also eventually carry the data types.
func LoadCodebase(thePath string) (CodeBase, error) {
	var cb CodeBase
	options, err := LoadCsv[Option, any](thePath+"cmd-line-options.csv", readCmdOption, nil)
	if err != nil {
		return CodeBase{}, err
	}

	endpoints, err := LoadCsv[endpoint, any](thePath+"cmd-line-endpoints.csv", readCmdEndpoint, nil)
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

	if len(cb.Commands) == 0 {
		return cb, fmt.Errorf("no commands were found in %s", thePath)
	}

	if len(cb.Structures) == 0 {
		return cb, fmt.Errorf("no structures were found in %s", thePath)
	}

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
				if f.Settings.GoOutput == "documentation only" {
					f.Settings.GoOutput = "<docs_only>"
					f.Settings.DisableGo = true
				}
				mapKey := strings.ToLower(class)
				structMap[mapKey] = f.Settings
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
				mapKey := strings.ToLower(class)
				structure := structMap[mapKey]
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
				structMap[mapKey] = structure
			}
		}
		return nil
	}); err != nil {
		return err
	}

	return nil
}

func (cb *CodeBase) FinishLoad(options []Option, endpoints []endpoint, structMap map[string]Structure) {
	theMap := make(map[string]Command)

	producesMap := make(map[string][]Production)

	// Create the structure array (and sort it by DocRoute) from the map
	cb.Structures = make([]Structure, 0, len(structMap))
	for _, st := range structMap {
		st.DocProducer = strings.Replace(st.DocProducer, " ", "", -1)
		st.Producers = strings.Split(st.DocProducer, ",")
		for _, producer := range st.Producers {
			producesMap[producer] = append(producesMap[producer], Production{st.Class})
		}
		st.cbPtr = cb
		for i := 0; i < len(st.Members); i++ {
			st.Members[i].stPtr = &st
		}
		cb.Structures = append(cb.Structures, st)
	}
	sort.Slice(cb.Structures, func(i, j int) bool {
		return cb.Structures[i].DocRoute < cb.Structures[j].DocRoute
	})

	// Attach each option to its command (i.e. its route)
	for _, opt := range options {
		route := strings.ToLower(opt.ApiRoute)
		c := Command{
			Options: append(theMap[route].Options, opt),
		}
		theMap[route] = c
	}

	// Enhance the commands with information from the endpoints data (could have been combined,
	// but this way we can keep the data separate and still have a single command object)
	for _, endpoint := range endpoints {
		route := strings.ToLower(endpoint.ApiRoute)
		if route == "" {
			route = strings.ToLower(endpoint.Group)
		}
		sort.Slice(producesMap[route], func(i, j int) bool {
			return producesMap[route][i].Value < producesMap[route][j].Value
		})
		c := Command{
			Route:        endpoint.ApiRoute,
			Group:        endpoint.Group,
			Description:  endpoint.Description,
			Options:      theMap[route].Options,
			Num:          endpoint.Num,
			Capabilities: endpoint.Capabilities,
			Usage:        endpoint.Usage,
			Folder:       endpoint.Folder,
			Tool:         endpoint.Tool,
			Summary:      endpoint.Summary,
			Productions:  producesMap[route],
			cbPtr:        cb,
		}

		if endpoint.ApiRoute != c.Route {
			logger.Fatal("route mismatch", endpoint.ApiRoute, c.Route)
		}
		if endpoint.Group != c.Group {
			logger.Fatal("group mismatch", endpoint.Group, c.Group)
		}
		if endpoint.Description != c.Description {
			logger.Fatal("description mismatch", endpoint.Description, c.Description)
		}

		theMap[route] = c
	}

	// Create a command array from the map and sort it by route within group
	cb.Commands = make([]Command, 0, len(theMap))
	for _, c := range theMap {
		c.Clean()
		cb.Commands = append(cb.Commands, c)
	}
	sort.Slice(cb.Commands, func(i, j int) bool {
		if cb.Commands[i].Route == cb.Commands[j].Route {
			return cb.Commands[i].Group < cb.Commands[j].Group
		}
		return cb.Commands[i].Route < cb.Commands[j].Route
	})

	file.StringToAsciiFile("src/dev_tools/goMaker/generated/codebase.json", cb.String())
}

type endpoint struct {
	Num           int    `json:"num" csv:"num"`
	Group         string `json:"group" csv:"group"`
	IsVisible     bool   `json:"is_visible" csv:"is_visible"`
	IsVisibleDocs bool   `json:"is_visible_docs" csv:"is_visible_docs"`
	Folder        string `json:"folder" csv:"folder"`
	ApiRoute      string `json:"api_route" csv:"api_route"`
	Tool          string `json:"tool" csv:"tool"`
	Summary       string `json:"summary" csv:"summary"`
	Usage         string `json:"usage" csv:"usage"`
	Capabilities  string `json:"capabilities" csv:"capabilities"`
	Description   string `json:"description" csv:"description"`
}

func (c endpoint) Validate() bool {
	return true
}

func readCmdEndpoint(ep *endpoint, data *any) (bool, error) {
	ep.Description = strings.ReplaceAll(ep.Description, "&#44;", ",")
	ep.Group = strings.Trim(ep.Group, wss)
	ep.Folder = strings.Trim(ep.Folder, wss)
	ep.ApiRoute = strings.Trim(ep.ApiRoute, wss)
	ep.Tool = strings.Trim(ep.Tool, wss)
	ep.Summary = strings.Trim(ep.Summary, wss)
	ep.Usage = strings.Trim(ep.Usage, wss)
	ep.Capabilities = strings.Trim(ep.Capabilities, wss)
	ep.Description = strings.Trim(ep.Description, wss)
	return true, nil
}

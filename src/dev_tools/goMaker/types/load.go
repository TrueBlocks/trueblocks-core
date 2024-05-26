package types

import (
	"fmt"
	"io/fs"
	"os"
	"path"
	"path/filepath"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// LoadCodebase loads the two csv files and returns the codebase which
// contains all the commands (each with its own set of options).
func LoadCodebase() (CodeBase, error) {
	if !checkRootDirectory() {
		return CodeBase{}, fmt.Errorf("this program must be run from the ./trueblocks-core folder")
	}

	thePath := "src/dev_tools/goMaker/templates/"
	if !file.FolderExists(thePath) {
		return CodeBase{}, fmt.Errorf("the path %s does not exist", thePath)
	}

	baseTypes, err := LoadCsv[Structure, any](thePath+"base-types.csv", readStructure, nil)
	if err != nil {
		return CodeBase{}, err
	}

	options, err := LoadCsv[Option, any](thePath+"cmd-line-options.csv", readCmdOption, nil)
	if err != nil {
		return CodeBase{}, err
	}

	var cb CodeBase
	structMap := make(map[string]Structure)
	err = cb.LoadStructures(thePath+"classDefinitions/", readStructure, structMap)
	if err != nil {
		return cb, err
	}

	err = cb.LoadMembers(thePath+"classDefinitions/", structMap)
	if err != nil {
		return cb, err
	}

	err = cb.FinishLoad(baseTypes, options, structMap)
	if err != nil {
		return cb, err
	}

	if len(cb.Commands) == 0 {
		return cb, fmt.Errorf("no commands were found in %s", thePath)
	}

	if len(cb.Structures) == 0 {
		return cb, fmt.Errorf("no structures were found in %s", thePath)
	}

	return cb, nil
}

func checkRootDirectory() bool {
	cwd, _ := os.Getwd()
	test := path.Join(cwd, "src/apps/chifra")
	_, err := os.Stat(test)
	return err == nil
}

func (cb *CodeBase) LoadStructures(thePath string, callBack func(*Structure, *any) (bool, error), structMap map[string]Structure) error {
	if err := filepath.Walk(thePath, func(path string, info fs.FileInfo, err error) error {
		if err != nil {
			return err
		}

		if info.IsDir() || !strings.HasSuffix(path, ".toml") {
			return nil
		}

		class := strings.TrimSuffix(filepath.Base(path), ".toml")
		type Tmp struct {
			Settings Structure `toml:"settings"` // don't change this, it won't parse
		}

		var f Tmp
		err = config.ReadToml(path, &f)
		if err != nil {
			return err
		}
		ok, err := callBack(&f.Settings, nil)
		if err != nil {
			return err
		}
		if ok {
			mapKey := strings.ToLower(class)
			structMap[mapKey] = f.Settings
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

		if info.IsDir() || !strings.HasSuffix(path, ".csv") {
			return nil
		}

		class := strings.TrimSuffix(filepath.Base(path), ".csv")
		mapKey := strings.ToLower(class)
		structure := structMap[mapKey]
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
		return nil
	}); err != nil {
		return err
	}

	return nil
}

func (cb *CodeBase) FinishLoad(baseTypes []Structure, options []Option, structMap map[string]Structure) error {
	cb.BaseTypes = baseTypes
	for i := 0; i < len(cb.BaseTypes); i++ {
		cb.BaseTypes[i].cbPtr = cb
	}

	producesMap := make(map[string][]*Structure)

	// Create the structure array (and sort it by DocRoute) from the map
	cb.Structures = make([]Structure, 0, len(structMap))
	for _, st := range structMap {
		for _, route := range st.Producers { // ProducedBy lists the routes that produce this structure
			producesMap[route] = append(producesMap[route], &st)
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
	routeMap := make(map[string]Command)
	for _, op := range options {
		route := strings.ToLower(op.Route)
		c := Command{
			Options: append(routeMap[route].Options, op),
		}
		routeMap[route] = c
	}

	// Enhance the commands with information from the endpoints data (could have been combined,
	// but this way we can keep the data separate and still have a single command object)
	for _, op := range options {
		if op.OptionType == "group" {
			c := Command{
				Group:       op.Group,
				Description: op.Description,
				Num:         op.Num,
				Summary:     op.Summary,
				cbPtr:       cb,
			}
			routeMap[op.Group] = c
		} else if op.OptionType == "command" {
			sort.Slice(producesMap[op.Route], func(i, j int) bool {
				return producesMap[op.Route][i].Class < producesMap[op.Route][j].Class
			})
			c := Command{
				Route:        op.Route,
				Group:        op.Group,
				Description:  op.Description,
				Options:      routeMap[op.Route].Options,
				Num:          op.Num,
				ReturnType:   op.ReturnType,
				Capabilities: op.Capabilities,
				Usage:        op.Usage,
				Folder:       op.Folder,
				Summary:      op.Summary,
				Productions:  producesMap[op.Route],
				cbPtr:        cb,
			}
			routeMap[op.Route] = c
		}
	}

	handlerMap := make(map[string][]Handler)
	for _, op := range options {
		if op.Handler > 0.0 {
			if len(handlerMap[op.Route]) == 0 {
				handlerMap[op.Route] = make([]Handler, 0)
			}
			handlerMap[op.Route] = append(handlerMap[op.Route], Handler{
				Position: op.Handler,
				Name:     FirstUpper(CamelCase(op.LongName)),
				Option:   &op,
			})
		}
	}

	for route, handlers := range handlerMap {
		if len(handlers) == 0 {
			continue
		}
		if strings.Contains(routeMap[route].Capabilities, "caching") {
			handlers = append(handlers, Handler{
				Position: 0.0,
				Name:     FirstUpper(CamelCase("Decache")),
				Option: &Option{
					ReturnType: "message",
					LongName:   "decache",
					Route:      route,
				},
			})
		}
		sort.Slice(handlers, func(i, j int) bool {
			return handlers[i].Position < handlers[j].Position
		})
		handlers[len(handlers)-1].Name = "Show"
		route := routeMap[route]
		route.Handlers = handlers
		routeMap[route.Route] = route
	}

	// Create a command array from the map and sort it by route within group
	cb.Commands = make([]Command, 0, len(routeMap))
	for _, c := range routeMap {
		c.Clean()
		if len(c.Group) > 0 {
			cb.Commands = append(cb.Commands, c)
		}
	}
	sort.Slice(cb.Commands, func(i, j int) bool {
		if cb.Commands[i].Route == cb.Commands[j].Route {
			return cb.Commands[i].Group < cb.Commands[j].Group
		}
		return cb.Commands[i].Route < cb.Commands[j].Route
	})

	if err := cb.Validate(); err != nil {
		return err
	}

	current := file.AsciiFileToString("src/dev_tools/goMaker/generated/codebase.json")
	file.StringToAsciiFile("src/dev_tools/goMaker/generated/codebase.json", cb.String())
	if current == cb.String() {
		return nil
	}

	if os.Getenv("TB_REMOTE_TESTING") == "true" {
		return nil
	}

	return fmt.Errorf("quitting: codebase.json has changed. Rerun the command to ignore this warning")
}

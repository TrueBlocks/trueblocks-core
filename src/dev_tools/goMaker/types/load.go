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

// LoadCodebase loads the two csv files and returns the codebase which
// contains all the commands (each with its own set of options).
func LoadCodebase() (CodeBase, error) {
	thePath, err := getTemplatesPath()
	if err != nil {
		return CodeBase{}, err
	}

	baseTypes, err := LoadCsv(filepath.Join(thePath, "base-types.csv"), readStructure, nil)
	if err != nil {
		return CodeBase{}, err
	}

	var cb CodeBase
	options, err := LoadCsv(filepath.Join(thePath, "cmd-line-options.csv"), readCmdOption, nil)
	if err != nil {
		return cb, err
	}
	err = checkForDups(options)
	if err != nil {
		return cb, err
	}

	structMap := make(map[string]Structure)
	err = cb.LoadStructures(filepath.Join(thePath, "classDefinitions/"), readStructure, structMap)
	if err != nil {
		return cb, err
	}

	err = cb.LoadMembers(filepath.Join(thePath, "classDefinitions/"), structMap)
	if err != nil {
		return cb, err
	}

	err = cb.FinishLoad(thePath, baseTypes, options, structMap)
	if err != nil {
		return cb, err
	}

	return cb, nil
}

func readStructure(st *Structure, data *any) (bool, error) {
	_ = data
	st.DocDescr = strings.ReplaceAll(st.DocDescr, "&#44;", ",")
	st.ProducedBy = strings.Replace(st.ProducedBy, " ", "", -1)
	st.Producers = strings.Split(st.ProducedBy, ",")
	st.ChildTabs = strings.Split(st.Children, ",")
	st.Class = strings.Trim(st.Class, " ")
	st.DocGroup = strings.Trim(st.DocGroup, " ")
	st.DocDescr = strings.Trim(st.DocDescr, " ")
	st.DocNotes = strings.Trim(st.DocNotes, " ")
	return true, nil
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
			Facets   []Facet   `toml:"facets"`
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
			// Normalize actions (strip -confirm suffix) while capturing confirmation flags
			for i := range f.Facets {
				f.Facets[i].NormalizeActions()
			}
			f.Settings.Facets = f.Facets // Copy facets into the Structure
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
		structure.Members, err = LoadCsv(path, readMember, nil)
		if err != nil {
			return err
		}
		dupMap := make(map[string]bool, len(structure.Members))
		for i := 0; i < len(structure.Members); i++ {
			if dupMap[structure.Members[i].Name] {
				return fmt.Errorf("duplicate member %s in class %s", structure.Members[i].Name, class)
			}
			structure.Members[i].Num = (i + 1)
			dupMap[structure.Members[i].Name] = true
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

func (cb *CodeBase) FinishLoad(unused string, baseTypes []Structure, options []Option, structMap map[string]Structure) error {
	_ = unused
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
		for i := 0; i < len(st.Facets); i++ {
			st.Facets[i].stPtr = &st
		}
		cb.Structures = append(cb.Structures, st)
	}
	sort.Slice(cb.Structures, func(i, j int) bool {
		if cb.Structures[i].DocRoute == cb.Structures[j].DocRoute {
			if cb.Structures[i].Name() == cb.Structures[j].Name() {
				si := ""
				for ii := 0; ii < len(cb.Structures[i].Members); ii++ {
					si += cb.Structures[i].Members[ii].Name
				}
				sj := ""
				for jj := 0; jj < len(cb.Structures[j].Members); jj++ {
					sj += cb.Structures[j].Members[jj].Name
				}
				// logger.Info("si", si, "sj", sj)
				return si < sj
			}
			return cb.Structures[i].Name() < cb.Structures[j].Name()
		}
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
		switch op.OptionType {
		case "group":
			VerboseLog("Processing group:", op.Group)
			c := Command{
				Group:       op.Group,
				Description: op.Description,
				Num:         op.Num,
				Summary:     op.Summary,
				cbPtr:       cb,
			}
			routeMap[op.Group] = c
		case "command":
			VerboseLog("Processing command:", op.Route, "in group:", op.Group)
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
				Attributes:   op.Attributes,
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
	crudMap := make(map[string]bool)
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
		} else if op.IsCrud() {
			crudMap[op.Route] = true
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
		if crudMap[route] {
			save := handlers[len(handlers)-1]
			handlers[len(handlers)-1] = Handler{
				Position: 0.0,
				Name:     FirstUpper(CamelCase("Crud")),
				Option: &Option{
					Route: route,
				},
			}
			handlers = append(handlers, save)
		}
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

	for _, st := range cb.Structures {
		st.checkHierarchy()
	}

	// Get the path to the generated folder and ensure it exists
	generatedPath := GetGeneratedPath()
	if err := file.EstablishFolder(generatedPath); err != nil {
		return fmt.Errorf("failed to create generated directory: %w", err)
	}

	codeBase := filepath.Join(generatedPath, "codebase.json")

	VerboseLog("Processing:", codeBase)
	current := file.AsciiFileToString(codeBase)
	_ = file.StringToAsciiFile(codeBase, cb.String())
	if current == cb.String() {
		VerboseLog("File unchanged:", codeBase)
		return nil
	}

	if os.Getenv("TB_REMOTE_TESTING") == "true" {
		return nil
	}

	return fmt.Errorf("quitting: %s has changed. Rerun the command to ignore this warning", codeBase)
}

func checkForDups(options []Option) error {
	dupMap := make(map[string]bool, len(options))
	for _, op := range options {
		key := op.Route + ":" + op.LongName
		if len(key) > 1 && dupMap[key] {
			return fmt.Errorf("duplicate option %s", key)
		}
		dupMap[key] = true
	}
	return nil
}

func (s *Structure) checkHierarchy() {
	if len(s.Parent) > 0 {
		found := false
		for _, st := range s.cbPtr.Structures {
			if Lower(st.Name()) == s.Parent {
				found = true
				break
			}
		}
		if !found {
			logger.Error("parent", s.Parent, "of", s.Class, "not found")
		}
	}

	for _, ch := range s.ChildTabs {
		if ch != "" {
			found := false
			for _, st := range s.cbPtr.Structures {
				if Plural(Lower(st.Name())) == ch {
					found = true
					break
				}
			}
			if !found {
				logger.Error("child", ch, "of", s.Class, "not found")
			}
		}
	}
}

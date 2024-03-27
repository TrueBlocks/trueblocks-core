package main

import (
	"fmt"
	"os"
	"sort"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/goMaker/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func main() {
	codeBase, err := LoadDefinitions()
	if err != nil {
		logger.Error(err)
		os.Exit(1)
	}

	slowPath(&codeBase)
	// if true {
	// 	fastPath(&codeBase)
	// }

	// array := []types.Structure{}
	// for _, structure := range codeBase.Structures {
	// 	array = append(array, structure)
	// }
	// sort.Slice(array, func(i, j int) bool {
	// 	return array[i].Class < array[j].Class
	// })
	// fmt.Println("[")
	// for i, structure := range array {
	// 	if i > 0 {
	// 		fmt.Println(",")
	// 	}
	// 	fmt.Printf("%s\n", structure.String())
	// }
	// fmt.Println("]")
}

func slowPath(codeBase *types.CodeBase) {
	for _, source := range goCodePerRoute {
		for _, c := range codeBase.Commands {
			if err := c.ProcessFile(source); err != nil {
				logger.Error(err)
				os.Exit(1)
			}
		}
	}
	for _, source := range goCodePerCodeBase {
		if err := codeBase.ProcessFile(source); err != nil {
			logger.Error(err)
			os.Exit(1)
		}
	}
	for _, source := range goTypes {
		for _, s := range codeBase.Structures {
			sort.Slice(s.Members, func(i, j int) bool {
				return s.Members[i].GoName() < s.Members[j].GoName()
			})
			if err := s.ProcessFile(source); err != nil {
				logger.Error(err)
				os.Exit(1)
			}
		}
	}
}

func fastPath(codeBase *types.CodeBase) {
	wg := sync.WaitGroup{}
	wg.Add(len(codeBase.Commands) * len(goCodePerRoute))
	wg.Add(len(goCodePerCodeBase))

	for _, source := range goCodePerRoute {
		for _, c := range codeBase.Commands {
			go func() {
				if err := c.ProcessFile(source); err != nil {
					logger.Error(err)
					os.Exit(1)
				}
				wg.Done()
			}()
		}
	}

	for _, source := range goCodePerCodeBase {
		go func() {
			if err := codeBase.ProcessFile(source); err != nil {
				logger.Error(err)
				os.Exit(1)
			}
			wg.Done()
		}()
	}
	wg.Wait()
}

// LoadDefinitions loads the definitions from the data-models folder
func LoadDefinitions() (types.CodeBase, error) {
	cwd, _ := os.Getwd()
	if !strings.HasSuffix(strings.Trim(cwd, "/"), "trueblocks-core") {
		return types.CodeBase{}, fmt.Errorf("this program must be run from the ./trueblocks-core folder")
	}

	thePath := "src/other/data-models/"
	if !file.FolderExists(thePath) {
		return types.CodeBase{}, fmt.Errorf("the path %s does not exist", thePath)
	}

	codeBase, err := types.LoadCodebase(thePath)
	if err != nil {
		return types.CodeBase{}, err
	}

	if len(codeBase.Commands) == 0 {
		return types.CodeBase{}, fmt.Errorf("no commands were found in %s", thePath)
	}

	if len(codeBase.Structures) == 0 {
		return types.CodeBase{}, fmt.Errorf("no structures were found in %s", thePath)
	}

	return codeBase, nil
}

// goCodePerRoute is the list of files to process per route
var goCodePerRoute = []string{
	// "sdk_go_route.go.tmpl",
	// // // - "sdk_python_src__route.py.tmpl",
	// // // - "sdk_typescript_src_paths_route.ts.tmpl",
	// "src_apps_chifra_cmd_route.go.tmpl",
	// "src_apps_chifra_internal_route_output.go.tmpl",
	// // "src_apps_chifra_internal_route_options.go.tmpl",
	// "src_apps_chifra_internal_route_doc.go.tmpl",
	// "src_apps_chifra_sdk_route.go.tmpl",
}

// goCodePerCodeBase is the list of files to process per code base
var goCodePerCodeBase = []string{
	// "src_apps_chifra_cmd_helpfile.go.tmpl",
	// "src_apps_chifra_pkg_version_string.go.tmpl",
}

var goTypes = []string{
	"src_apps_chifra_pkg_types_type.go.tmpl",
	"src_apps_chifra_internal_route_types_type.go.tmpl",
}

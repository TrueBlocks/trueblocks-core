package main

import (
	"fmt"
	"os"
	"strings"

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

	// fmt.Println(codeBase.String())
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

	codeBase, err := types.LoadCommands(thePath)
	if err != nil {
		return types.CodeBase{}, err
	}

	if len(codeBase.Commands) == 0 {
		return types.CodeBase{}, fmt.Errorf("no commands were found in %s", thePath)
	}

	return codeBase, nil
}

// goCodePerRoute is the list of files to process per route
var goCodePerRoute = []string{
	"sdk_go_route.go.tmpl",
	// // // - "sdk_python_src__route.py.tmpl",
	// // // - "sdk_typescript_src_paths_route.ts.tmpl",
	// "src_apps_chifra_cmd_route.go.tmpl",
	"src_apps_chifra_internal_route_output.go.tmpl",
	// "src_apps_chifra_internal_route_options.go.tmpl",
	"src_apps_chifra_sdk_route.go.tmpl",
}

// goCodePerCodeBase is the list of files to process per code base
var goCodePerCodeBase = []string{
	"src_apps_chifra_cmd_helpfile.go.tmpl",
	"src_apps_chifra_pkg_version_string.go.tmpl",
}

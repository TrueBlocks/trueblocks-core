package main

import (
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

func main() {
	fmt.Println("Generating Go code for TrueBlocks disabled")
	os.Exit(1)
	// codeBase, err := LoadDefinitions()
	// if err != nil {
	// 	logger.Error(err)
	// 	os.Exit(1)
	// }

	// for _, source := range goCodePerRoute {
	// 	for _, c := range codeBase.Commands {
	// 		c.processFile(source)
	// 	}
	// }

	// for _, source := range goCodePerCodeBase {
	// 	codeBase.processFile(source)
	// }
}

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

	codeBase, err := LoadCommands(thePath)
	if err != nil {
		return CodeBase{}, err
	}

	if len(codeBase.Commands) == 0 {
		return CodeBase{}, fmt.Errorf("no commands were found in %s", thePath)
	}

	return codeBase, nil
}

// goCodePerRoute is the list of files to process per route
var goCodePerRoute = []string{
	"templates/sdk_go_route.go.tmpl",
	// "templates/sdk_python_src__route.py.tmpl",
	// "templates/sdk_typescript_src_paths_route.ts.tmpl",
	"templates/src_apps_chifra_cmd_route.go.tmpl",
	"templates/src_apps_chifra_internal_route_output.go.tmpl",
	"templates/src_apps_chifra_internal_route_options.go.tmpl",
	"templates/src_apps_chifra_sdk_route.go.tmpl",
}

// goCodePerCodeBase is the list of files to process per code base
var goCodePerCodeBase = []string{
	"templates/src_apps_chifra_cmd_helpfile.go.tmpl",
	"templates/src_apps_chifra_pkg_version_string.go.tmpl",
}

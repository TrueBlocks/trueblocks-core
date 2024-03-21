package main

import (
	"fmt"
	"os"
	"strings"

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
			if err := c.processFile(source); err != nil {
				logger.Error(err)
				os.Exit(1)
			}
		}
	}

	for _, source := range goCodePerCodeBase {
		if err := codeBase.processFile(source); err != nil {
			logger.Error(err)
			os.Exit(1)
		}
	}
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

func shouldProcess(source, route string) (bool, error) {
	if strings.Contains(source, "sdk_") {
		if route == "explore" || route == "daemon" || route == "scrape" {
			return false, nil
		}
	}

	if !file.FileExists(source) {
		return false, fmt.Errorf("file does not exist %s", source)
	}

	return true, nil
}

func convertToDestPath(source, route string) string {
	dest := strings.Replace(source, templateFolder, "", -1)
	dest = strings.Replace(dest, ".tmpl", "", -1)
	dest = strings.Replace(dest, "_route_", "/"+route+"/", -1)
	dest = strings.Replace(dest, "__route", "_+route", -1)
	dest = strings.Replace(dest, "route.go", route+".go", -1)
	dest = strings.Replace(dest, "route.py", route+".py", -1)
	dest = strings.Replace(dest, "route.ts", route+".ts", -1)
	dest = strings.Replace(dest, "_", "/", -1)
	dest = strings.Replace(dest, "+", "_", -1)
	return dest
}

var templateFolder = "src/dev_tools/goMaker/templates"

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

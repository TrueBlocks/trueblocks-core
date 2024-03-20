package main

import (
	"fmt"
	"os"
)

func main() {
	thePath := "../src/other/data-models/"
	codeBase, err := LoadCommands(thePath)
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Println(codeBase.String())
	os.Exit(0)

	goCodePerRoute := []string{
		"templates/sdk_go_route.go",
		// "templates/sdk_python_src__route.py",
		// "templates/sdk_typescript_src_paths_route.ts",
		"templates/src_apps_chifra_cmd_route.go",
		"templates/src_apps_chifra_internal_route_output.go",
		"templates/src_apps_chifra_internal_route_options.go",
		"templates/src_apps_chifra_sdk_route.go",
	}

	for _, source := range goCodePerRoute {
		for _, c := range codeBase.Commands {
			c.processFile(source)
		}
	}

	goCodePerCodeBase := []string{
		"templates/src_apps_chifra_cmd_helpfile.go",
		"templates/src_apps_chifra_pkg_version_string.go",
	}

	for _, source := range goCodePerCodeBase {
		codeBase.processFile(source)
	}
}

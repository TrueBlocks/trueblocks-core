package main

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/goMaker/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func main() {
	codeBase, err := types.LoadDefinitions()
	if err != nil {
		logger.Error(err)
		os.Exit(1)
	}
	codeBase.Generate(cbTemplates, routeTemplates, typeTemplates)
}

// cbTemplates are the templates for the code base
var cbTemplates = []string{
	"src_apps_chifra_cmd_helpfile.go.tmpl",
	"src_apps_chifra_pkg_version_string.go.tmpl",
	"docs_content_api_openapi.yaml.tmpl",
}

// routeTemplates are the templates for the routes
var routeTemplates = []string{
	"sdk_go_route.go.tmpl",
	"sdk_python_src__route.py.tmpl",
	// "sdk_typescript_src_paths_route.ts.tmpl",
	"src_apps_chifra_cmd_route.go.tmpl",
	"src_apps_chifra_internal_route_output.go.tmpl",
	"src_apps_chifra_internal_route_options.go.tmpl",
	"src_apps_chifra_internal_route_doc.go.tmpl",
	"src_apps_chifra_sdk_route.go.tmpl",
}

// typeTemplates are the templates for the data models
var typeTemplates = []string{
	"src_apps_chifra_pkg_types_type.go.tmpl",
	"src_apps_chifra_internal_route_types_type.go.tmpl",
}

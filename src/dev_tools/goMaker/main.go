package main

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/goMaker/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func main() {
	codeBase, err := types.LoadCodebase()
	if err != nil {
		logger.Error(err)
		os.Exit(1)
	}

	codeBase.Generate(cbTemplates)
}

// generators are the templates for the codebase
var cbTemplates = []types.Generator{
	{
		Against: "codebase",
		Templates: []string{
			"src_apps_chifra_internal_daemon_routes.go.tmpl",
			"src_apps_chifra_cmd_helpfile.go.tmpl",
			"src_apps_chifra_pkg_version_string.go.tmpl",
			"docs_content_api_openapi.yaml.tmpl",
			"src_dev+tools_goMaker_generated_readme+chifra.md.tmpl",
		},
	},
	{
		Against: "routes",
		Templates: []string{
			"src_dev+tools_goMaker_generated_readme+route.md.tmpl",
			"sdk_go_route.go.tmpl",
			"sdk_python_src_+route.py.tmpl",
			// "sdk_typescript_src_paths_route.ts.tmpl",
			"src_apps_chifra_cmd_route.go.tmpl",
			"src_apps_chifra_internal_route_output.go.tmpl",
			"src_apps_chifra_internal_route_options.go.tmpl",
			"src_apps_chifra_internal_route_doc.go.tmpl",
			"src_apps_chifra_internal_route_README.md.tmpl",
			"src_apps_chifra_sdk_route.go.tmpl",
		},
	},
	{
		Against: "types",
		Templates: []string{
			"src_dev+tools_goMaker_generated_model+type.md.tmpl",
			"src_apps_chifra_pkg_types_type.go.tmpl",
			"src_apps_chifra_internal_route_types_type.go.tmpl",
		},
	},
	{
		Against: "groups",
		Templates: []string{
			"docs_content_reason_group.md.tmpl",
		},
	},
}

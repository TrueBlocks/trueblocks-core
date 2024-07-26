package types

import (
	"encoding/json"
	"fmt"
	"path/filepath"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

type Generator struct {
	Against   string   `json:"against"`
	Templates []string `json:"templates"`
}

// Generate generates the code for the codebase using the given templates.
func (cb *CodeBase) Generate() {
	generatedPath := GetGeneratedPath()
	file.EstablishFolder(generatedPath)

	generators, err := getGenerators()
	if err != nil {
		logger.Fatal(err)
	}

	for _, generator := range generators {
		switch generator.Against {
		case "codebase":
			for _, source := range generator.Templates {
				if err := cb.ProcessFile(source, "", ""); err != nil {
					logger.Fatal(err)
				}
			}
		case "groups":
			for _, source := range generator.Templates {
				for _, group := range cb.GroupList("") {
					if err := cb.ProcessGroupFile(source, group.GroupName(), "readme"); err != nil {
						logger.Fatal(err)
					}
				}
			}
			for _, source := range generator.Templates {
				for _, group := range cb.GroupList("") {
					if err := cb.ProcessGroupFile(source, group.GroupName(), "model"); err != nil {
						logger.Fatal(err)
					}
				}
			}
		case "routes":
			for _, source := range generator.Templates {
				for _, c := range cb.Commands {
					if err := c.ProcessFile(source, "", ""); err != nil {
						logger.Fatal(err)
					}
				}
			}
		case "types":
			for _, source := range generator.Templates {
				for _, s := range cb.Structures {
					sort.Slice(s.Members, func(i, j int) bool {
						return s.Members[i].SortName() < s.Members[j].SortName()
					})
					if !s.DisableGo {
						if err := s.ProcessFile(source, "", ""); err != nil {
							logger.Fatal(err)
						}
					}
				}
			}
		}
	}
	logger.Info(colors.Green + "Done..." + strings.Repeat(" ", 120) + colors.Off + "\033[K")
}

// getGenerators returns the generators we will be using
func getGenerators() ([]Generator, error) {
	thePath, err := getTemplatesPath()
	if err != nil {
		return []Generator{}, err
	}
	generatorsPath := filepath.Join(thePath, "generators/") + "/"

	theMap := make(map[string][]string)
	vFunc := func(file string, vP any) (bool, error) {
		if strings.HasSuffix(file, ".tmpl") {
			file = strings.ReplaceAll(file, generatorsPath, "")
			if strings.Contains(file, "/") {
				parts := strings.Split(file, "/")
				theMap[parts[0]] = append(theMap[parts[0]], file)
			}
		}
		return true, nil
	}

	walk.ForEveryFileInFolder(generatorsPath, vFunc, nil)

	ret := []Generator{}
	for against, templates := range theMap {
		g := Generator{
			Against: against,
		}
		sort.Strings(templates)
		for _, template := range templates {
			template = strings.ReplaceAll(template, g.Against+"/", "")
			g.Templates = append(g.Templates, template)
		}
		ret = append(ret, g)
	}

	bytes, _ := json.MarshalIndent(ret, "", "  ")
	fmt.Println(string(bytes))

	return ret, nil
}

// generators are the templates for the codebase
// var cbTemplates = []Generator{
// 	{
// 		Against: "codebase",
// 		Templates: []string{
// 			"docs_content_api_openapi.yaml.tmpl",
// 			"src_apps_chifra_cmd_helpfile.go.tmpl",
// 			"src_apps_chifra_internal_daemon_routes.go.tmpl",
// 			"src_apps_chifra_pkg_types_modeler.go.tmpl",
// 			"src_apps_chifra_pkg_version_string.go.tmpl",
// 			"src_dev+tools_goMaker_generated_readme+chifra.md.tmpl",
// 		},
// 	},
// 	{
// 		Against: "routes",
// 		Templates: []string{
// 			"sdk_python_src_+route.py.tmpl",
// 			"sdk_route+internal.go.tmpl",
// 			"sdk_route.go.tmpl",
// 			"sdk_typescript_src_paths_route.ts.tmpl",
// 			"src_apps_chifra_cmd_route.go.tmpl",
// 			"src_apps_chifra_internal_route_README.md.tmpl",
// 			"src_apps_chifra_internal_route_doc.go.tmpl",
// 			"src_apps_chifra_internal_route_options.go.tmpl",
// 			"src_apps_chifra_internal_route_output.go.tmpl",
// 			"src_apps_chifra_sdk_route.go.tmpl",
// 			"src_dev+tools_goMaker_generated_readme+route.md.tmpl",
// 			"src_dev+tools_sdkFuzzer_route.go.tmpl",
// 		},
// 	},
// 	{
// 		Against: "types",
// 		Templates: []string{
// 			"sdk_typescript_src_types_type.ts.tmpl",
// 			"src_apps_chifra_pkg_types_type.go.tmpl",
// 			"src_dev+tools_goMaker_generated_model+type.md.tmpl",
// 		},
// 	},
// 	{
// 		Against: "groups",
// 		Templates: []string{
// 			"docs_content_reason_group.md.tmpl",
// 		},
// 	},
// }

package types

import (
	"os"
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
	VerboseLog("Starting code generation process")

	// Validate that the necessary files and folders exist
	if err := cb.isValidSetup(); err != nil {
		logger.Fatal(err)
	}

	// Before we start, we need to verify that the validators are in place
	cb.verifyValidators()

	generatedPath := GetGeneratedPath()
	VerboseLog("Creating generated code directory at", generatedPath)
	_ = file.EstablishFolder(generatedPath)

	generators, err := getGenerators()
	if err != nil {
		logger.Fatal(err)
	}

	VerboseLog("Processing generators")
	for _, generator := range generators {
		VerboseLog("Processing", generator.Against, "templates")
		switch generator.Against {
		case "codebase":
			for _, source := range generator.Templates {
				VerboseLog("Processing codebase template:", source)
				if err := cb.ProcessFile(source, "", ""); err != nil {
					logger.Fatal(err)
				}
			}
		case "groups":
			for _, source := range generator.Templates {
				VerboseLog("Processing group template:", source)
				for _, group := range cb.GroupList("") {
					VerboseLog("  - For group:", group.GroupName())
					if err := cb.ProcessGroupFile(source, group.GroupName(), "readme"); err != nil {
						logger.Fatal(err)
					}
				}
			}
			for _, source := range generator.Templates {
				VerboseLog("Processing group model template:", source)
				for _, group := range cb.GroupList("") {
					if err := cb.ProcessGroupFile(source, group.GroupName(), "model"); err != nil {
						logger.Fatal(err)
					}
				}
			}
		case "routes":
			for _, source := range generator.Templates {
				VerboseLog("Processing route template:", source)
				for _, c := range cb.Commands {
					VerboseLog("  - For command:", c.Route)
					if err := c.ProcessFile(source, "", ""); err != nil {
						logger.Fatal(err)
					}
				}
			}
		case "types":
			for _, source := range generator.Templates {
				VerboseLog("Processing type template:", source)
				for _, s := range cb.Structures {
					sort.Slice(s.Members, func(i, j int) bool {
						return s.Members[i].SortName() < s.Members[j].SortName()
					})
					if !s.DisableGo {
						VerboseLog("  - For type:", s.Name())
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
	VerboseLog("Looking for templates in:", generatorsPath)

	theMap := make(map[string][]string)
	vFunc := func(file string, vP any) (bool, error) {
		_ = vP
		isPartial := strings.HasSuffix(file, ".partial.tmpl")
		isTemplate := strings.HasSuffix(file, ".tmpl") && !isPartial
		filter := os.Getenv("TB_GENERATOR_FILTER")
		if len(filter) > 0 && !strings.Contains(file, filter) {
			return true, nil
		}
		if isTemplate {
			VerboseLog("  Found template:", file)
			file = strings.ReplaceAll(file, generatorsPath, "")
			if strings.Contains(file, string(os.PathSeparator)) {
				parts := strings.Split(file, string(os.PathSeparator))
				theMap[parts[0]] = append(theMap[parts[0]], file)
			}
		}
		return true, nil
	}

	_ = walk.ForEveryFileInFolder(generatorsPath, vFunc, nil)

	ret := []Generator{}
	for against, templates := range theMap {
		VerboseLog("  Creating generator for:", against, "with templates:", templates)
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
	sort.Slice(ret, func(i, j int) bool {
		sort.Strings(ret[i].Templates)
		return ret[i].Against < ret[j].Against
	})

	return ret, nil
}

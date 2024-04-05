package types

import (
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type Generator struct {
	Against   string
	Templates []string
}

// Generate generates the code for the codebase using the given templates.
func (cb *CodeBase) Generate(generators []Generator) {
	for _, gen := range generators {
		switch gen.Against {
		case "codebase":
			for _, source := range gen.Templates {
				if err := cb.ProcessFile(source); err != nil {
					logger.Fatal(err)
				}
			}
		case "groups":
			for _, group := range cb.ModelList() {
				for _, source := range gen.Templates {
					if err := cb.ProcessGroupFile(source, group.GroupName()); err != nil {
						logger.Fatal(err)
					}
				}
			}
		case "routes":
			for _, source := range gen.Templates {
				for _, c := range cb.Commands {
					if err := c.ProcessFile(source); err != nil {
						logger.Fatal(err)
					}
				}
			}
		case "types":
			for _, source := range gen.Templates {
				for _, s := range cb.Structures {
					sort.Slice(s.Members, func(i, j int) bool {
						return s.Members[i].SortName() < s.Members[j].SortName()
					})
					if err := s.ProcessFile(source); err != nil {
						logger.Fatal(err)
					}
				}
			}
		}
	}

	logger.Info()
	logger.Info(colors.Green + "Done..." + strings.Repeat(" ", 120) + colors.Off + "\033[K")
}

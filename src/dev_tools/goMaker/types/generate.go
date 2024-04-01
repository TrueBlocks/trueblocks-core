package types

import (
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// Generate generates the code for the codebase using the given templates.
func (cb *CodeBase) Generate(cbTmpls, routeTmpls, typeTmpls []string) {
	// var wg sync.WaitGroup

	// wg.Add(len(cbTmpls))
	for _, source := range cbTmpls {
		// go func() {
		// 	defer wg.Done()
		if err := cb.ProcessFile(source); err != nil {
			logger.Fatal(err)
		}
		// }()
	}

	// wg.Add(len(routeTmpls) * len(cb.Commands))
	for _, source := range routeTmpls {
		for _, c := range cb.Commands {
			// go func() {
			// 	defer wg.Done()
			if err := c.ProcessFile(source); err != nil {
				logger.Fatal(err)
			}
			// }()
		}
	}

	// wg.Add(len(typeTmpls) * len(cb.Structures))
	for _, source := range typeTmpls {
		for _, s := range cb.Structures {
			// go func() {
			// 	defer wg.Done()
			sort.Slice(s.Members, func(i, j int) bool {
				return s.Members[i].SortName() < s.Members[j].SortName()
			})
			if err := s.ProcessFile(source); err != nil {
				logger.Fatal(err)
			}
			// }()
		}
	}

	logger.Info(colors.Green + "Done..." + strings.Repeat(" ", 120) + colors.Off + "\033[K")
	// wg.Wait()
}

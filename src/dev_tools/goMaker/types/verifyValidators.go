package types

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func (cb *CodeBase) verifyValidators() {
	cwd, _ := os.Getwd()
	for _, cmd := range cb.Commands {
		path := filepath.Join(cwd, "src/apps/chifra/internal/", cmd.Route, "validate.go")
		for _, opts := range cmd.Options {
			if len(opts.Enums) > 0 {
				contents := file.AsciiFileToString(path)
				want := strings.Join(opts.Enums, "|")
				if !strings.Contains(contents, want) {
					logger.Fatal(fmt.Sprintf("Missing enum validator (%s) for %s", want, path))
				}
			}
		}
	}
}

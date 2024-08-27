package types

import (
	"fmt"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func (cb *CodeBase) verifyValidators() {
	cwd, _ := os.Getwd()
	for _, cmd := range cb.Commands {
		path := filepath.Join(cwd, "src/apps/chifra/internal/", cmd.Route, "validate.go")
		for _, opts := range cmd.Options {
			if ok, wanted := ValidateEnums(path, opts.Enums); !ok {
				logger.Fatal(fmt.Sprintf("Missing enum validator (%s) for %s", wanted, path))
			}
		}
	}
}

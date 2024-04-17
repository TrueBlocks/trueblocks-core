package main

import (
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (t *TestCase) CmdTest(mode string) (string, error) {
	tmpFn, goldFn, envFn := t.GetOutputPaths(mode)
	tmpFn = filepath.Base(tmpFn) + ".tmp"
	defer os.Remove(tmpFn)

	cmd := "cd " + filepath.Dir(goldFn) + " && "
	if file.FileExists(envFn) {
		lines := file.AsciiFileToLines(envFn)
		for _, line := range lines {
			cmd += line + " "
			logger.Info("Env:", line)
		}
	}

	cmd += "NO_COLOR=true "
	cmd += "TEST_MODE=true chifra "
	cmd += t.Route + " "
	cmd += t.CmdOptions + " "
	cmd += ">>" + tmpFn + " "
	cmd += "2>&1"

	// logger.Info(cmd)

	utils.System(cmd)
	return file.AsciiFileToString(tmpFn), nil
}

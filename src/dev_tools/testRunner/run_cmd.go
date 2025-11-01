package main

import (
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/utils"
)

func (t *TestCase) CmdTest() (string, error) {
	_, goldFn, envFn, outputFn := t.GetOutputPaths("cmd")
	// tmpFn := goldFn + ".tmp"
	tmpFn := getTempFilePath(goldFn)
	// fmt.Println(tmpFn)
	defer func() {
		os.Remove(tmpFn)
	}()

	cmd := "cd " + filepath.Dir(goldFn) + " && "
	if file.FileExists(envFn) {
		lines := file.AsciiFileToLines(envFn)
		for _, line := range lines {
			if !strings.HasPrefix(line, "#") {
				cmd += strings.ReplaceAll(line, " ", "") + " "
			}
		}
	}
	cmd += "NO_COLOR=true "
	cmd += "TEST_MODE=true "
	cmd += "chifra "
	if t.Route != "chifra" && t.Tool != "chifra" {
		cmd += t.Route + " "
	}
	cmd += t.CmdOptions + " "
	cmd += ">" + tmpFn + " "
	cmd += "2>&1"
	utils.System(cmd)
	contents := file.AsciiFileToString(tmpFn)
	if file.FileExists(outputFn) {
		contents += strings.ReplaceAll(`----
Results in ./OUTPUT_NAME
`, "OUTPUT_NAME", filepath.Base(outputFn))
		contents += file.AsciiFileToString(outputFn)
	}
	contents = strings.Trim(contents, "\n\r")
	return contents, nil
}

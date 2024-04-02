package types

import (
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/goMaker/codeWriter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

func (s *Structure) ProcessFile(source string) error {
	if s.DisableGo {
		return nil
	}

	cwd, _ := os.Getwd()
	source = filepath.Join(cwd, templateFolder, source)
	if ok, err := shouldProcess(source, s.Class); err != nil {
		return err
	} else if !ok {
		return nil
	}

	dest := ""
	sourceIn := strings.Contains(source, "internal")
	destIn := strings.Contains(s.GoOutput, "internal")
	if destIn && !sourceIn || sourceIn && !destIn {
		// fmt.Println("Mismatch", s.Class)
		return nil
	} else if destIn {
		s.Route = grabRoute(s.GoOutput)
		dest = convertToDestPath(source, s.Route, s.Name())
		dest = strings.Replace(dest, "/types/", "/types_", -1)
		// return nil
	} else {
		dest = convertToDestPath(source, "", s.Name())
	}

	tmplName := source
	tmpl := file.AsciiFileToString(source)
	result := s.executeTemplate(tmplName, tmpl)
	dest = strings.Replace(dest, "//src/apps/chifra/pkg/types/", "/"+s.GoOutput+"/types_", -1)
	return codeWriter.WriteCode(dest, result)
}

func grabRoute(dest string) string {
	if !strings.Contains(dest, "/internal/") {
		return ""
	}
	parts := strings.Split(dest, "/")
	return parts[len(parts)-1]
}

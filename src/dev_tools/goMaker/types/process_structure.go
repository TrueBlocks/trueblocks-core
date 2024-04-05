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
	isSourceInternal := strings.Contains(source, "internal")
	isSourceGenerated := strings.Contains(source, "generated")
	isDestInternal := strings.Contains(s.GoOutput, "internal")

	if !isSourceGenerated && (isDestInternal && !isSourceInternal || isSourceInternal && !isDestInternal) {
		return nil
	} else if isDestInternal {
		s.Route = grabRoute(s.GoOutput)
		dest = convertToDestPath(source, s.Route, s.Name(), "", "")
		dest = strings.Replace(dest, "/types/", "/types_", -1)
	} else {
		dest = convertToDestPath(source, "", s.Name(), "", "")
	}

	tmpl := file.AsciiFileToString(source)
	result := s.executeTemplate(source, tmpl)
	dest = strings.Replace(dest, "/src/apps/chifra/pkg/types/", "/"+s.GoOutput+"/types_", -1)
	_, err := codeWriter.WriteCode(dest, result)
	return err
}

func grabRoute(dest string) string {
	if !strings.Contains(dest, "/internal/") {
		return ""
	}
	parts := strings.Split(dest, "/")
	return parts[len(parts)-1]
}

func readStructure(st *Structure, data *any) (bool, error) {
	st.DocDescr = strings.ReplaceAll(st.DocDescr, "&#44;", ",")
	st.ProducedBy = strings.Replace(st.ProducedBy, " ", "", -1)
	st.Producers = strings.Split(st.ProducedBy, ",")
	return true, nil
}

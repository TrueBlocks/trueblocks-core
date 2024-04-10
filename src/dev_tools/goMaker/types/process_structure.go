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

	dest := convertToDestPath(source, "", s.Name(), "", "")
	tmpl := file.AsciiFileToString(source)
	result := s.executeTemplate(source, tmpl)
	dest = strings.Replace(dest, "/src/apps/chifra/pkg/types/", "/src/apps/chifra/pkg/types/types_", -1)
	_, err := codeWriter.WriteCode(dest, result)
	return err
}

func readStructure(st *Structure, data *any) (bool, error) {
	st.DocDescr = strings.ReplaceAll(st.DocDescr, "&#44;", ",")
	st.ProducedBy = strings.Replace(st.ProducedBy, " ", "", -1)
	st.Producers = strings.Split(st.ProducedBy, ",")
	return true, nil
}

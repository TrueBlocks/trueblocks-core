package types

import (
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/goMaker/codeWriter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// ProcessFile processes a single file, applying the template to it and
// writing the result to the destination.
func (s *Structure) ProcessFile(source string) error {
	if s.DisableGo {
		return nil
	}

	cwd, _ := os.Getwd()
	source = filepath.Join(cwd, GetTemplatePath(), source)
	if ok, err := shouldProcess(source, s.Class); err != nil {
		return err
	} else if !ok {
		return nil
	}

	tmpl := file.AsciiFileToString(source)
	dest := convertToDestPath(source, "", s.Name(), "", "")
	dest = strings.ReplaceAll(dest, "/src/apps/chifra/pkg/types/", "/src/apps/chifra/pkg/types/types_")
	result := s.executeTemplate(source, tmpl)
	_, err := codeWriter.WriteCode(dest, result)
	return err
}

func readStructure(st *Structure, data *any) (bool, error) {
	st.DocDescr = strings.ReplaceAll(st.DocDescr, "&#44;", ",")
	st.ProducedBy = strings.Replace(st.ProducedBy, " ", "", -1)
	st.Producers = strings.Split(st.ProducedBy, ",")
	st.Class = strings.Trim(st.Class, " ")
	st.DocGroup = strings.Trim(st.DocGroup, " ")
	st.DocDescr = strings.Trim(st.DocDescr, " ")
	st.DocNotes = strings.Trim(st.DocNotes, " ")
	return true, nil
}

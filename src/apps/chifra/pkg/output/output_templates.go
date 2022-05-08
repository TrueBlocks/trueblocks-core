package output

import (
	"html/template"
	"reflect"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func getFields(t *reflect.Type, format string) (fields []string, sep string, quote string) {
	if (*t).Kind() != reflect.Struct {
		logger.Fatal((*t).Name() + " is not a structure")
	}
	for i := 0; i < (*t).NumField(); i++ {
		fields = append(fields, utils.MakeFirstLowerCase((*t).Field(i).Name))
	}
	sep = "\t"
	quote = ""
	if format == "csv" {
		sep = ","
		quote = "\""
	}
	return fields, sep, quote
}

func GetHeader(t *reflect.Type, format string) string {
	fields, sep, quote := getFields(t, format)
	var sb strings.Builder
	for i, field := range fields {
		if i > 0 {
			sb.WriteString(sep)
		}
		sb.WriteString(quote + field + quote)
	}
	return sb.String()
}

func GetRowTemplate(t *reflect.Type, format string) (*template.Template, error) {
	fields, sep, quote := getFields(t, format)
	var sb strings.Builder
	for i, field := range fields {
		if i > 0 {
			sb.WriteString(sep)
		}
		sb.WriteString(quote + "{{." + utils.MakeFirstLowerCase(field) + "}}" + quote)
	}
	tt, err := template.New("").Parse(sb.String() + "\n")
	return tt, err
}

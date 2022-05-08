package output

import (
	"html/template"
	"reflect"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func getFields(t *reflect.Type, format string, header bool) (fields []string, sep string, quote string) {
	if (*t).Kind() != reflect.Struct {
		logger.Fatal((*t).Name() + " is not a structure")
	}
	for i := 0; i < (*t).NumField(); i++ {
		fn := (*t).Field(i).Name
		if header {
			fields = append(fields, utils.MakeFirstLowerCase(fn))
		} else {
			fields = append(fields, fn)
		}
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
	fields, sep, quote := getFields(t, format, true)
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
	fields, sep, quote := getFields(t, format, false)
	var sb strings.Builder
	for i, field := range fields {
		if i > 0 {
			sb.WriteString(sep)
		}
		sb.WriteString(quote + "{{." + field + "}}" + quote)
	}
	tt, err := template.New("").Parse(sb.String() + "\n")
	return tt, err
}

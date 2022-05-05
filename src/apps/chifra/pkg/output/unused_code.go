package output

// Table makes it easier to output tabular data to the console
// type Table struct {
// 	Writer *tabwriter.Writer
// 	target *os.File
// }

// New sets up the default writer and target for a table
// func (t *Table) New() {
// 	t.Writer = tabwriter.NewWriter(os.Stdout, 0, 0, 2, ' ', 0)
// 	t.target = os.Stdout
// }

// Header prints table header to the writer
// func (t *Table) Header(header []string) {
// 	fmt.Fprintln(t.Writer, strings.Join(header, "\t"))
// }

// Row prints data as table cells in one row
// func (t *Table) Row(cells []string) {
// 	for _, cell := range cells {
// 		fmt.Fprint(t.Writer, cell, "\t")
// 	}
// 	fmt.Fprint(t.Writer, "\n")
// }

// Print flushes the Writer, which will print the table
// func (t *Table) Print() error {
// 	return t.Writer.Flush()
// }

// func MakeFirstUpperCase(s string) string {
// 	if len(s) < 2 {
// 		return strings.ToLower(s)
// 	}
// 	bts := []byte(s)
// 	lc := bytes.ToUpper([]byte{bts[0]})
// 	rest := bts[1:]
// 	return string(bytes.Join([][]byte{lc, rest}, nil))
// }

// TODO: Can we use golang templates for speed?
// func getFields(t *reflect.Type) (fields []string, sep string, quote string) {
// 	for i := 0; i < (*t).NumField(); i++ {
// 		fields = append(fields, MakeFirstLowerCase((*t).Field(i).Name))
// 	}
// 	sep = "\t"
// 	quote = ""
// 	if Format == "csv" {
// 		sep = ","
// 		quote = "\""
// 	}
// 	return fields, sep, quote
// }

// func GetHeader(t *reflect.Type) string {
// 	if (*t).Kind() != reflect.Struct {
// 		logger.Fatal((*t).Name() + " is not a structure")
// 	}

// 	fields, sep, quote := getFields(t)
// 	var sb strings.Builder
// 	for i, field := range fields {
// 		if i > 0 {
// 			sb.WriteString(sep)
// 		}
// 		sb.WriteString(quote + field + quote)
// 	}
// 	return sb.String()
// }

// func GetRowTemplate(t *reflect.Type) (*template.Template, error) {
// 	fields, sep, quote := getFields(t)
// 	var sb strings.Builder
// 	for i, field := range fields {
// 		if i > 0 {
// 			sb.WriteString(sep)
// 		}
// 		sb.WriteString(quote + "{{." + MakeFirstUpperCase(field) + "}}" + quote)
// 	}
// 	// fmt.Println(sb.String() + "\n")
// 	tt, err := template.New("").Parse(sb.String() + "\n")
// 	return tt, err
// }
//const b = strings.Builder{}

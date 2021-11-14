package output

// // This type is used to carry CSV layout information
// type CsvFormatted struct {
// 	Header  []string
// 	Content [][]string
// }

// // CsvFo rmatter turns a type into CSV string. It uses custom code instead of
// // Go's encoding/csv to maintain compatibility with C++ output, which
// // quotes each item. encoding/csv would double-quote a quoted string...
// func CsvFo rmatter(i interface{}) ([]byte, error) {
// 	records, err := ToStringRecords(i, true)
// 	if err != nil {
// 		return nil, err
// 	}
// 	result := []string{}
// 	// We have to join records in one row with a ","
// 	for _, row := range records {
// 		result = append(result, strings.Join(row, ","))
// 	}

// 	// Now we need to join all rows with a newline. We also add one
// 	// final newline to be concise with both Go's encoding/csv and C++
// 	// version
// 	return []byte(
// 		strings.Join(result, "\n") + "\n",
// 	), nil
// }

package fourbytes

import (
	"fmt"
	"os"
	"path/filepath"
)

func logError(filename string, err string) {
	fmt.Println("[", filename, "]:", err)
}

// Verify checks if the files written match the data
func Verify(chunks *map[string]mapRecord, outDir string) {
	done := 0

	for filePath, record := range *chunks {
		done++
		if done%10000 == 0 {
			fmt.Printf("%d...\n", done)
		}
		if len(record.sigRecords) == 0 {
			logError(filePath, "empty")
			continue
		}
		file, err := os.OpenFile(filepath.Join(outDir, filePath), os.O_RDONLY, 0)
		if err != nil {
			logError(filePath, err.Error())
			file.Close()
			continue
		}
		// read data from the file
		sigs, strs, err := readAll(file)
		if err != nil {
			logError(filePath, err.Error())
			file.Close()
			continue
		}
		// check lengths
		got := len(record.sigRecords)
		if len(sigs) != got {
			logError(filePath, fmt.Sprintf("wrong number of sigs. Got %d, expected %d", got, len(sigs)))
		}
		got = len(record.strRecords)
		if len(strs) != got {
			logError(filePath, fmt.Sprintf("wrong number of strings. Got %d, expected %d", got, len(strs)))
		}

		// These take lots of time
		// for _, sigRecord := range record.sigRecords {
		// 	found := false
		// 	for _, sig := range sigs {
		// 		if sig.Signature.Hex() == sigRecord.Signature.Hex() {
		// 			found = true
		// 			break
		// 		}
		// 	}
		// 	if !found {
		// 		logError(filePath, fmt.Sprintf("Signature '%s' missing", sigRecord.Signature.Hex()))
		// 	}
		// }
		// for _, strRecord := range record.strRecords {
		// 	found := false
		// 	for _, str := range strs {
		// 		if str.Value == strRecord.Value {
		// 			found = true
		// 			break
		// 		}
		// 	}
		// 	if !found {
		// 		logError(filePath, fmt.Sprintf("String '%s' missing", strRecord.Value))
		// 	}
		// }

		file.Close()
	}
}

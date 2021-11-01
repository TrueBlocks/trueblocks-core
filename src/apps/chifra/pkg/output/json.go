package output

import (
	"encoding/json"
	"fmt"
	"os"
)

var apiWriter = os.Stdout

func PrintJson(serializable interface{}) error {
	response := map[string]interface{}{
		"data": serializable,
	}
	marshalled, err := json.MarshalIndent(response, "", "    ")
	if err != nil {
		return err
	}
	fmt.Fprintln(apiWriter, string(marshalled))

	return nil
}

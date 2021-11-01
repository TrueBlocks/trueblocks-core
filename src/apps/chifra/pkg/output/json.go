package output

import (
	"encoding/json"
	"fmt"
	"os"
)

// PrintJson marshals its arguments and prints JSON in a standardized
// format
func PrintJson(serializable interface{}) error {
	response := map[string]interface{}{
		"data": serializable,
	}
	marshalled, err := json.MarshalIndent(response, "", "    ")
	if err != nil {
		return err
	}
	fmt.Fprintln(os.Stdout, string(marshalled))

	return nil
}

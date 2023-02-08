package types

import "encoding/json"

func CompressMap(input map[string]any) (result string, err error) {
	marshalled, err := json.Marshal(&input)
	if err != nil {
		return
	}
	result = string(marshalled)
	return
}

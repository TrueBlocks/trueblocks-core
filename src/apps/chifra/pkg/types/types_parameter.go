package types

import "fmt"

type RawParameter interface{}
type SimpleParameter struct {
	ParameterType string            `json:"parameterType"`
	Name          string            `json:"name"`
	StrDefault    string            `json:"strDefault"`
	Value         string            `json:"value"`
	Indexed       bool              `json:"indexed"`
	InternalType  string            `json:"internalType"`
	Components    []SimpleParameter `json:"components"`
	Unused        bool              `json:"unused"`
	IsFlags       uint64            `json:"is_flags"`
}

func (s *SimpleParameter) Raw() *RawParameter {
	return nil
}

func (s *SimpleParameter) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	return Model{
		Data: map[string]any{
			"type":         s.ParameterType,
			"name":         s.Name,
			"indexed":      s.Indexed,
			"internalType": s.InternalType,
			"components":   s.Components,
		},
		// We only output SimpleParameter when using JSON format, so we don't need order
	}
}

// DisplayName returns parameter name if defined, or a default name "val_" + index
func (s *SimpleParameter) DisplayName(index int) string {
	if s.Name != "" {
		return s.Name
	}
	return "val_" + fmt.Sprint(index)
}

func ParametersToMap(params []SimpleParameter) (result map[string]string) {
	result = make(map[string]string)
	for index, param := range params {
		if param.Value == "0x" || param.Value == "0x0" {
			continue
		}
		result[param.DisplayName(index)] = param.Value
	}
	return
}

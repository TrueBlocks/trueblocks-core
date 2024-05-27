package types

type Modeler interface {
	Model(chain, format string, verbose bool, extraOpts map[string]any) Model
}

type Model struct {
	Data  map[string]any
	Order []string
}

// EXISTING_CODE
// EXISTING_CODE

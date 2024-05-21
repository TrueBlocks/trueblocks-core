package types

type Modeler[RT interface{}] interface {
	Model(chain, format string, verbose bool, extraOptions map[string]any) Model
}

type Model struct {
	Data  map[string]any
	Order []string
}

type RawModeler interface{}

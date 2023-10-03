package types

type Modeler[RT RawData] interface {
	Model(chain, format string, verbose bool, extraOptions map[string]any) Model
	Raw() *RT
}

type Model struct {
	Data  map[string]any
	Order []string
}

type RawModeler interface{}

type RawData interface {
	RawBlock | RawTransaction | RawReceipt | RawLog |
		RawTrace | RawTraceAction | RawTraceResult |
		RawFunction | RawParameter | RawAppearance | RawModeler
}

package types

import "io"

type Modeler[RT RawData] interface {
	Model(showHidden bool, format string, extraOptions map[string]any) Model
	Raw() *RT
}

type Cacheable interface {
	WriteTo(w io.Writer) (n int64, err error)
	ReadFrom(r io.Reader) (n int64, err error)
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

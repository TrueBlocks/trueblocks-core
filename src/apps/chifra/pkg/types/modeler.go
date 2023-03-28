package types

import "io"

type Modeler[Raw RawData] interface {
	Model(showHidden bool, format string, extraOptions map[string]any) Model
	Raw() *Raw
}

type Cacheable[Raw RawData] interface {
	WriteTo(w io.Writer) (n int64, err error)
	ReadFrom(r io.Reader) (n int64, err error)
}

type Model struct {
	Data  map[string]any
	Order []string
}

// TODO: BOGUS - The auto code generation should check that all auto generated fields are included here
type RawData interface {
	RawReceipt | RawWhenCount | RawNamedBlock | RawBlock | RawBlockCount | RawTraceAction |
		RawTraceResult | RawTrace | RawTraceCount | RawFunction | RawParameter | RawAppearance |
		RawReportCheck
}

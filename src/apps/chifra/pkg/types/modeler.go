package types

type Modeler[RT Streamable] interface {
	Model(chain, format string, verbose bool, extraOpts map[string]any) Model
}

type Model struct {
	Data  map[string]any
	Order []string
}

type Streamable interface {
	struct{} | // needed for auto-gen formatting only
		// EXISTING_CODE
		// EXISTING_CODE
		Abi |
		Appearance |
		AppearanceTable |
		Block |
		BlockCount |
		Bounds |
		CacheItem |
		Chain |
		ChunkAddress |
		ChunkBloom |
		ChunkIndex |
		ChunkPin |
		ChunkRecord |
		ChunkStats |
		Function |
		IpfsPin |
		LightBlock |
		Log |
		Manifest |
		Message |
		Monitor |
		MonitorClean |
		Name |
		NamedBlock |
		Parameter |
		RawBlock |
		RawLightBlock |
		RawTransaction |
		Receipt |
		ReportCheck |
		Result |
		Slurp |
		State |
		Statement |
		Status |
		Timestamp |
		TimestampCount |
		Token |
		Trace |
		TraceAction |
		TraceCount |
		TraceFilter |
		TraceResult |
		Transaction |
		Withdrawal
}

// EXISTING_CODE
// EXISTING_CODE

package types

type Modeler[RT Streamable] interface {
	Model(chain, format string, verbose bool, extraOpts map[string]interface{}) Model
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
		BlockCount |
		Bounds |
		CacheItem |
		ChunkAddress |
		ChunkBloom |
		ChunkIndex |
		ChunkPin |
		ChunkRecord |
		ChunkStats |
		Function |
		IpfsPin |
		Log |
		Manifest |
		Message |
		Monitor |
		MonitorClean |
		Name |
		NamedBlock |
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
		TraceCount |
		Withdrawal
}

// EXISTING_CODE
// EXISTING_CODE

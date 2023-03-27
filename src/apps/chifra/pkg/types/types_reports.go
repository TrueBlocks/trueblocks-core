package types

type ReportCheck struct {
	Reason     string   `json:"reason"`
	VisitedCnt uint32   `json:"nVisits,omitempty"`
	CheckedCnt uint32   `json:"nTests,omitempty"`
	SkippedCnt uint32   `json:"nSkipps,omitempty"`
	PassedCnt  uint32   `json:"nPasses,omitempty"`
	FailedCnt  uint32   `json:"nFails,omitempty"`
	Result     string   `json:"result,omitempty"`
	MsgStrings []string `json:"msgStrings,omitempty"`
}

type ReportChunks struct {
	Start         uint64  `json:"start"`
	End           uint64  `json:"end"`
	NAddrs        uint32  `json:"nAddrs"`
	NApps         uint32  `json:"nApps"`
	NBlocks       uint64  `json:"nBlocks"`
	NBlooms       uint32  `json:"nBlooms"`
	RecWid        uint64  `json:"recWid"`
	BloomSz       int64   `json:"bloomSz"`
	ChunkSz       int64   `json:"chunkSz"`
	AddrsPerBlock float64 `json:"addrsPerBlock"`
	AppsPerBlock  float64 `json:"appsPerBlock"`
	AppsPerAddr   float64 `json:"appsPerAddr"`
	Ratio         float64 `json:"ratio"`
}

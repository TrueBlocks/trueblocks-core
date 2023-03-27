package types

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

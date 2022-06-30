package types

type CleanReport struct {
	Addr     string `json:"addr"`
	SizeThen uint32 `json:"sizeThen"`
	SizeNow  uint32 `json:"sizeNow"`
	Dups     uint32 `json:"dupsRemoved"`
}

type RepairReport struct {
	Status    string `json:"status"`
	Range     string `json:"range,omitempty"`
	BloomHash string `json:"bloomHash,omitempty"`
	IndexHash string `json:"indexHash,omitempty"`
}

type CheckReport struct {
	Reason     string   `json:"reason"`
	VisitedCnt uint32   `json:"nVisited,omitempty"`
	CheckedCnt uint32   `json:"nChecked,omitempty"`
	SkippedCnt uint32   `json:"nSkipped,omitempty"`
	PassedCnt  uint32   `json:"nPassed,omitempty"`
	FailedCnt  uint32   `json:"nFailed,omitempty"`
	ErrorStrs  []string `json:"errorStrs,omitempty"`
	Result     string   `json:"result,omitempty"`
}

type ChunksReport struct {
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

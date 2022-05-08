package types

type NamedBlock struct {
	BlockNumber uint64 `json:"blockNumber"`
	TimeStamp   uint64 `json:"timestamp"`
	Date        string `json:"date"`
	Name        string `json:"name"`
}

type CleanReport struct {
	Addr     string `json:"addr"`
	SizeThen uint32 `json:"sizeThen"`
	SizeNow  uint32 `json:"sizeNow"`
	Dups     uint32 `json:"dupsRemoved"`
}

type SimpleAppearance struct {
	Address          string `json:"address"`
	BlockNumber      uint32 `json:"blockNumber"`
	TransactionIndex uint32 `json:"transactionIndex"`
}

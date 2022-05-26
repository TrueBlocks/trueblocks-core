package types

type NamedBlock struct {
	BlockNumber uint64 `json:"blockNumber"`
	TimeStamp   uint64 `json:"timestamp"`
	Date        string `json:"date"`
	Name        string `json:"name,omitempty"`
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

type SimpleLog struct {
	Address          string   `json:"address"`
	LogIndex         uint32   `json:"logIndex"`
	BlockNumber      uint32   `json:"blockNumber,omitempty"`
	TransactionIndex uint32   `json:"transactionIndex,omitempty"`
	Timestamp        uint64   `json:"timestamp,omitempty"`
	Topics           []string `json:"topics"`
	Data             string   `json:"data,omitempty"`
	CompressedLog    string   `json:"compressedLog,omitempty"`
}

type SimpleReceipt struct {
	// BlockNumber      uint32      `json:"blockNumber,omitempty"`
	// TransactionIndex uint32      `json:"transactionIndex,omitempty"`
	// Hash             string      `json:"hash,omitempty"`
	ContractAddress   string      `json:"contractAddress"`
	GasUsed           uint64      `json:"gasUsed"`
	EffectiveGasPrice uint64      `json:"effectiveGasPrice"`
	Logs              []SimpleLog `json:"logs,omitempty"`
	Status            *uint32     `json:"status"`
	IsError           bool        `json:"isError,omitempty"`
}

type Function struct {
	Encoding  string `json:"encoding,omitempty"`
	Signature string `json:"signature,omitempty"`
}

type SimpleMonitor struct {
	Address     string `json:"address"`
	NRecords    int    `json:"nRecords"`
	FileSize    int64  `json:"fileSize"`
	LastScanned uint32 `json:"lastScanned"`
}

type SimplePinList struct {
	FileName  string `json:"fileName"`
	BloomHash string `json:"bloomHash"`
	IndexHash string `json:"indexHash"`
}

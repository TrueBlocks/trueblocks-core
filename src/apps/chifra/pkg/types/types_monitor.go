package types

type RawMonitor interface{}

type SimpleMonitor struct {
	Address     string `json:"address"`
	NRecords    int    `json:"nRecords"`
	FileSize    int64  `json:"fileSize"`
	LastScanned uint32 `json:"lastScanned"`
}

func (s *SimpleMonitor) Write(p []byte) (n int, err error) {
	return 0, nil
}

func (s *SimpleMonitor) Read(p []byte) (n int, err error) {
	return 0, nil
}

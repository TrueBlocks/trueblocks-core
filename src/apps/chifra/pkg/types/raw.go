package types

// These types are used to unmarshal responses directly from node. We want
// to allow users to get node output directly if they want to. In other
// cases, we want to transform the data so that it is easier to work with
// in the codebase and makes more sense where printing.

type RawLog struct {
	Address          string   `json:"address"`
	Topics           []string `json:"topics"`
	Data             string   `json:"data"`
	BlockNumber      string   `json:"blockNumber"`
	TransactionHash  string   `json:"transactionHash"`
	TransactionIndex string   `json:"transactionIndex"`
	BlockHash        string   `json:"blockHash"`
	LogIndex         string   `json:"logIndex"`
	Removed          bool     `json:"removed"`
}

package configtypes

import "encoding/json"

type UnchainedGroup struct {
	PreferredPublisher string `json:"preferredPublisher" toml:"preferredPublisher,omitempty" comment:"The default publisher of the index if none other is provided"`
	SmartContract      string `json:"smartContract" toml:"smartContract,omitempty" comment:"The address of the current version of the Unchained Index"`
}

func (s *UnchainedGroup) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

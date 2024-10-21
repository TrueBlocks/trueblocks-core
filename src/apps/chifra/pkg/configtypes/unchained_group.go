package configtypes

type UnchainedGroup struct {
	PreferredPublisher string `toml:"preferredPublisher,omitempty" comment:"The default publisher of the index if none other is provided"`
	SmartContract      string `toml:"smartContract,omitempty" comment:"The address of the current version of the Unchained Index"`
}

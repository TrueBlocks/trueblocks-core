package configtypes

var defaultConfig = Config{
	Settings: SettingsGroup{
		// The location of the per chain caches
		CachePath: "",
		// The location of the per chain unchained indexes
		IndexPath: "",
		// The default chain to use if none is provided
		DefaultChain: "mainnet",
	},
	Pinning: PinningGroup{
		// The pinning gateway to query when downloading the unchained index
		GatewayUrl: "",
		// The local endpoint for the IPFS daemon
		LocalPinUrl: "http://localhost:5001",
		// The remote endpoint for pinning on Pinata
		RemotePinUrl: "https://api.pinata.cloud/pinning/pinFileToIPFS",
	},
	Unchained: UnchainedGroup{
		// The default publisher of the index if none other is provided
		PreferredPublisher: "publisher.unchainedindex.eth",
		// V2: The address of the current version of the Unchained Index
		SmartContract: "0x0c316b7042b419d07d343f2f4f5bd54ff731183d",
	},
}

func NewConfig(cachePath, indexPath, defaultIpfs string) Config {
	ret := defaultConfig
	ret.Settings.CachePath = cachePath
	ret.Settings.IndexPath = indexPath
	ret.Pinning.GatewayUrl = defaultIpfs
	return ret
}

package config

var defaultConfig = &ConfigFile{
	Settings: settingsGroup{
		// The location of the per chain caches
		CachePath: cachePath,
		// The location of the per chain unchained indexes
		IndexPath: indexPath,
		// The default chain to use if none is provided
		DefaultChain: "mainnet",
	},
	Pinning: pinningGroup{
		// The pinning gateway to query when downloading the unchained index
		GatewayUrl: defaultIpfsGateway,
		// The local endpoint for the IPFS daemon
		LocalPinUrl: "http://localhost:5001",
		// The remote endpoint for pinning on Pinata
		RemotePinUrl: "https://api.pinata.cloud/pinning/pinFileToIPFS",
	},
	Unchained: unchainedGroup{
		// The default publisher of the index if none other is provided
		PreferredPublisher: "publisher.unchainedindex.eth",
		// V2: The address of the current version of the Unchained Index
		SmartContract: "0x0c316b7042b419d07d343f2f4f5bd54ff731183d",
	},
}

package configtypes

type PinningGroup struct {
	GatewayUrl   string `toml:"gatewayUrl" comment:"The pinning gateway to query when downloading the unchained index"`
	LocalPinUrl  string `toml:"localPinUrl" comment:"The local endpoint for the IPFS daemon"`
	RemotePinUrl string `toml:"remotePinUrl" comment:"The remote endpoint for pinning on Pinata"`
}

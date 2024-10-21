package configtypes

type PinningGroup struct {
	GatewayUrl   string `json:"gatewayUrl" toml:"gatewayUrl" comment:"The pinning gateway to query when downloading the unchained index"`
	LocalPinUrl  string `json:"localPinUrl" toml:"localPinUrl" comment:"The local endpoint for the IPFS daemon"`
	RemotePinUrl string `json:"remotePinUrl" toml:"remotePinUrl" comment:"The remote endpoint for pinning on Pinata"`
}

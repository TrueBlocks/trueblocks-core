package rpcClient

// TODO: move this to ABI package when we port ABIs
var tokenRelated = map[string]bool{
	// functions
	"0x095ea7b3": true, // approve(address spender, uint256 value)
	"0xa9059cbb": true, // transfer(address from, uint256 to);
	"0x23b872dd": true, // transferFrom(address from, address to, uint256 value)
	"0xb3e1c718": true, // _safeMint(address, uint256)
	"0x6a4f832b": true, // _safeMint(address, uint256, bytes)
	"0xa1448194": true, // safeMint(address, uint256)
	"0x8832e6e3": true, // safeMint(address, uint256, bytes)
	"0x4e6ec247": true, // _mint(address, uint256)
	"0x4cd4edcb": true, // _mint(address, uint256, bytes, bytes)
	"0x40c10f19": true, // mint(address, uint256)
	"0xcfa84fc1": true, // mint(uint256, address[], uint256[])
	"0x278d9c41": true, // mintEventToManyUsers(uint256, address[])
	"0x78b27221": true, // mintFungible(uint256, address[], uint256[])
	"0xf9419088": true, // mintNonFungible(uint256, address[])
	"0xf190ac5f": true, // mintToAddresses(address[], uint256)
	"0xa140ae23": true, // mintToken(uint256, address)
	"0xf980f3dc": true, // mintUserToManyEvents(uint256[], address)
	"0x14004ef3": true, // multimint(address[], uint256[])
	"0x6a627842": true, // mint(address)
	"0xa0712d68": true, // mint(uint256)
	// topics
	"0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef": true, // Transfer(address from, address to, uint256 value)
	"0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925": true, // Approval(address owner, address spender, uint256 value)
	"0xd4735d920b0f87494915f556dd9b54c8f309026070caea5c737245152564d266": true, // Transfer(bytes32 node, address owner)
	"0x30385c845b448a36257a6a1716e6ad2e1bc2cbe333cde1e69fe849ad6511adfe": true, // Minted(address,uint256)
}

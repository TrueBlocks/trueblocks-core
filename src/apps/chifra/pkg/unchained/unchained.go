package unchained

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"

const (
	Schemas         = "QmUou7zX2g2tY58LP1A2GyP5RF9nbJsoxKTp299ah3svgb"                     // IPFS hash of the specification for the Unchained Index
	HeaderMagicHash = "0x81ae14ba68e372bc9bd4a295b844abd8e72b1de10fcd706e624647701d911da1" // V2: Internal hash for the index chunks. The keccek256 of the manifest version
	ReadHashName_V2 = "manifestHashMap"                                                    // V2: The name of the function to read the hash
)

const (
	address_V2         = "0x0c316b7042b419d07d343f2f4f5bd54ff731183d" // V2: The address of the current version of the Unchained Index
	preferredPublisher = "0xf503017d7baf7fbc0fff7492b751025c6a78179b" // V2: Us
)

// GetUnchainedIndexAddress returns the address of the current version of the Unchained Index
func GetUnchainedIndexAddress() base.Address {
	return base.HexToAddress(address_V2)
}

// GetPreferredPublisher returns the address of the preferred publisher
func GetPreferredPublisher() base.Address {
	return base.HexToAddress(preferredPublisher)
}

// Unchained Index Version 1.0
// Address: 0x0c316b7042b419d07d343f2f4f5bd54ff731183d
//
// encoding		name			params						       returns
// ---------------------------------------------------------------------------------------------------
// 0xed88c68e	donate          ()
// 0x1fee5cd2	publishHash     (string chain, string hash)
// 0x7087e4bd	manifestHashMap (address publisher, string chain)  returns (string hash)
// 0x8da5cb5b	owner           ()                                 returns (string owner)
// 0xa6f9dae1	changeOwner     (address newOnwer)                 returns (address oldOwner)
//
// encoding																name           params
// ----------------------------------------------------------------------------------------------------------------------------------------
// 0x09732eb896ea819849e5f867dd547ed8ba539c5d9b68be96055d96f1c85abaa7	HashPublished  (address publisher, string chain, string hash)
// 0x8d68ee857f05464080096b8fbaa1603c09866dfccd25bb348209b1188e60add2	DonationSent   (address from, uint256 amount, uint256 ts)
// 0xb532073b38c83145e3e5135377a08bf9aab55bc0fd7c1179cd4fb995d2a5159c	OwnerChanged   (address oldOwner, address newOwner)

//
// Unchained Index Version 0.3
// Address      = "0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd" // V1: The address of the current version of the Unchained Index
// ReadHash     = "0x337f3f32"                                 // V1: The fourbyte needed to read the current manifest hash from the smart contract
// ReadHashName = "manifestHash"                               // V1: The name of the function to read the hash
// PublishHash  = "0x8da5cb5b"                                 // V1: The fourbyte needed to publish the hash to the smart contract
//
// encoding		name			params						       returns
// ---------------------------------------------------------------------------------------------------
// 0x337f3f32	manifestHash    ()                                 returns (string hash)
// 0x8da5cb5b	owner           ()                                 returns (string owner)
// 0xa6f9dae1	changeOwner     (address newOnwer)                 returns (address oldOwner)
// 0x8f1f9902	publishHash     (string hash)
//
// encoding																name           params
// ----------------------------------------------------------------------------------------------------------------------------------------
// 0xb532073b38c83145e3e5135377a08bf9aab55bc0fd7c1179cd4fb995d2a5159c	OwnerChanged   (address oldOwner, address newOwner)
// 0x0488fc5a0e346a47f90cfa4d25262d1a430445fa176c4b7000f005bc2f474b73	HashPublished  (string hash)

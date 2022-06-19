package unchained

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// TODO: BOGUS Should these values be in a config file?
// TODO: BOGUS Should these values be per chain?
const (
	Schemas            = "Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2"
	Databases          = "Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2"
	Address            = "0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd"
	ReadHash           = "0x337f3f32"
	ReadHashName       = "manifestHash"
	PublishHash        = "0x8da5cb5b"
	Address_V2         = "0x0c316b7042b419d07d343f2f4f5bd54ff731183d"
	ReadHash_V2        = "0x7087e4bd"
	ReadHashName_V2    = "manifestHashMap"
	PublishHash_V2     = "0x1fee5cd2"
	PreferredPublisher = "0xf503017d7baf7fbc0fff7492b751025c6a78179b"
)

func PrintHeader(chain string, testMode bool) {
	logger.Log(logger.Info, "schemas:", Schemas)
	logger.Log(logger.Info, "databases:", Databases)
	if !NewUnchained(chain) {
		logger.Log(logger.Info, "unchainedAddress:", Address)
		logger.Log(logger.Info, "unchainedReadHash:", ReadHash)
		logger.Log(logger.Info, "unchainedPublishHash:", PublishHash)
	} else {
		logger.Log(logger.Info, "unchainedAddress:", Address_V2)
		logger.Log(logger.Info, "unchainedReadHash:", ReadHash_V2)
		logger.Log(logger.Info, "unchainedPublishHash:", PublishHash_V2)
	}
	if !testMode {
		logger.Log(logger.Info, "manifestLocation:", config.GetPathToChainConfig(chain)) // order matters
		logger.Log(logger.Info, "unchainedIndexFolder:", config.GetPathToIndex(chain))   // order matters
	}
}

func NewUnchained(chain string) bool {
	return os.Getenv("NEW_UNCHAINED") == "true" || chain == "sepolia" || chain == "gnosis"
}

// dweb:/ipfs/QmUni86YFR1a322VjJhk1etqjTT6wMErTpijXMRQTfPaX7
// Unchained_V1_Addr        = "0x0c316b7042b419d07d343f2f4f5bd54ff731183d"
// Unchained_V1_ReadHash    = "0x7087e4bd"
// Unchained_V1_PublishHash = "0x1fee5cd2"

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
// Address: 0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd
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

package base

var (
	Byzantium      = "byzantium"
	Constantinople = "constantinople"
	London         = "london"
	Merge          = "merge"
	Shanghai       = "shanghai"
)

var knownBlocks = map[string]map[string]Blknum{
	"mainnet": {
		Byzantium:      4370000,
		Constantinople: 7280000,
		London:         12965000,
		Merge:          15537393,
		Shanghai:       17034870,
	},
	"sepolia": {
		Merge:    1450409,
		Shanghai: 2990908,
	},
}

func KnownBlock(chain, blockName string) Blknum {
	return knownBlocks[chain][blockName]
}

package base

// These purposefully chosen baddresses are used to indicate that the transaction is a prefund
// and uncle reward, or a mining reward. They are not real addresses, but are used to indicate
// that the transaction is not a normal transaction. They are not (currently) indexed.
var (
	PrefundSender     = HexToAddress("0x0000000000000000000000000050726566756e64") // The word "Prefund" in hex
	BlockRewardSender = HexToAddress("0x0000000000000000000000000000004d696e6572") // The word "Miner" in hex
	UncleRewardSender = HexToAddress("0x000000000000000000000000000000556e636c65") // The word "Uncle" in hex
	WithdrawalSender  = HexToAddress("0x000000000000000000005769746864726177616C") // The word "Withdrawal" in hex
	SkippedSender     = HexToAddress("0x00000000000000000000000000536B6970706564") // The word "Skipped" in hex
)

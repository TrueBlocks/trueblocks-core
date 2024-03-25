package base

type Gas = uint64
type Blknum = uint64
type Txnum = uint64
type Topic = string
type Timestamp = int64

// TODO: This is probably the same as String (could be removed)

func IsFinal(latestTs, blockTs Timestamp) bool {
	// TODO: This is not consistent with they way we determine unripe in the scraper, for example.
	var min = Timestamp(5)
	var s = Timestamp(60)
	var pendingPeriod = min * s
	var diff = latestTs - blockTs
	return diff >= pendingPeriod
}

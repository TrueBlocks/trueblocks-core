package sdk

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"

var sdkTestMode = false

func init() {
	sdkTestMode = base.IsTestMode()
}

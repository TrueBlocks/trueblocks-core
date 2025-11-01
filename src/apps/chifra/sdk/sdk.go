package sdk

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"

var sdkTestMode = false

func init() {
	sdkTestMode = base.IsTestMode()
}

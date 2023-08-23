package index

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type Reason struct {
	App     *types.SimpleAppearance
	Address *base.Address
	Reason  string
}

// TODO: BOGUS

func GetNeighbors(app *types.SimpleAppearance) ([]Reason, error) {
	reasons := make([]Reason, 0)
	reasons = append(reasons, Reason{app, &app.Address, "self"})
	return reasons, nil
}

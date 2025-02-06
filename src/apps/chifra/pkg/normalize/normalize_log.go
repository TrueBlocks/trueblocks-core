package normalize

import (
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var ErrNormalization = errors.New("normalization error")
var ErrNonIndexedTransfer = fmt.Errorf("non-indexed transfer")

func NormalizeTransferOrApproval(log *types.Log) (*types.Log, error) {
	if len(log.Topics) < 3 {
		// Transfer(address _from, address _to, uint256 _tokenId) - no indexed topics
		// Transfer(address indexed _from, address indexed _to, uint256 _value) - two indexed topics
		// Transfer(address indexed _from, address indexed _to, uint256 indexed _tokenId) - three indexed topics
		// TODO: This may be a transfer. Returning here is wrong. What this means is that
		// TODO:the some of the transfer's data is not indexed. Too short topics happens
		// TODO: (sometimes) because the ABI says that the data is not index, but it is
		// TODO: or visa versa. In either case, we get the same topic0. We need to
		// TODO: attempt both with and without indexed parameters. See issues/1366.
		// TODO: We could fix this and call back in recursively...
		return nil, ErrNonIndexedTransfer
	}

	return log, nil
}

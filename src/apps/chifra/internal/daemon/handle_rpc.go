package daemonPkg

import (
	"net"
	"net/rpc"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

const sockPath = "/tmp/trueblocks.sock"

type Rpc struct{}

func (r *Rpc) ReadName(address *base.Address, reply *types.SimpleName) error {
	logger.Info("Handling ReadName")
	n := names.ReadCustomName(*address)
	*reply = *n
	return nil
}

func (opts *DaemonOptions) HandleRpc() error {
	if err := os.RemoveAll(sockPath); err != nil {
		return err
	}

	// TODO(names): temp
	_, err := names.LoadNamesMap("mainnet", names.Regular|names.Custom, nil)
	if err != nil {
		return err
	}

	r := new(Rpc)
	rpc.Register(r)
	listener, err := net.Listen("unix", sockPath)
	if err != nil {
		return err
	}
	rpc.Accept(listener)
	return nil
}

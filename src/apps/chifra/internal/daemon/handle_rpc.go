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

func (r *Rpc) ReadCustomName(address *base.Address, reply *types.SimpleName) error {
	logger.Info("Handling ReadName")
	n := names.ReadCustomName(*address)
	*reply = *n
	return nil
}

type SearchNamesArgs struct {
	Parts names.Parts
	Terms []string
}

func (r *Rpc) SearchNames(args *SearchNamesArgs, reply *[]types.SimpleName) error {
	logger.Info("Handling SearchNames")
	found, err := names.LoadNamesArray("mainnet", args.Parts, names.SortByAddress, args.Terms)
	if err != nil {
		return err
	}
	*reply = found
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

package daemonPkg

import (
	"context"
	"fmt"
	"net"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/proto"
	"google.golang.org/grpc"
)

type chifraRpcServer struct {
	proto.UnimplementedNamesServer
}

// Search looks up name by given terms
func (g *chifraRpcServer) Search(ctx context.Context, request *proto.SearchRequest) (*proto.SearchResponse, error) {
	logger.Info("Handling SearchNames")
	found, err := names.LoadNamesArray("mainnet", names.Parts(request.GetParts()), names.SortByAddress, request.GetTerms())
	if err != nil {
		return nil, err
	}
	pnames := make([]*proto.Name, 0, len(found))

	for _, name := range found {
		pnames = append(pnames, name.ToMessage())
	}

	return &proto.SearchResponse{
		Names: pnames,
	}, nil
}

// SearchStream is like Search, but it streams the response
func (g *chifraRpcServer) SearchStream(request *proto.SearchRequest, stream proto.Names_SearchStreamServer) error {
	logger.Info("Handling SearchStream")
	found, err := names.LoadNamesArray("mainnet", names.Parts(request.GetParts()), names.SortByAddress, request.GetTerms())
	if err != nil {
		return err
	}

	for _, name := range found {
		if err := name.Send(stream); err != nil {
			return err
		}
	}

	return nil
}

func (opts *DaemonOptions) HandleRpc() error {
	if err := os.RemoveAll(proto.SocketAddress()); err != nil {
		return err
	}

	if err := names.PrepareRpc(); err != nil {
		return err
	}

	rpcServer := grpc.NewServer()
	proto.RegisterNamesServer(rpcServer, &chifraRpcServer{})

	listener, err := net.Listen("unix", proto.SocketAddress())
	if err != nil {
		return err
	}

	if err := rpcServer.Serve(listener); err != nil {
		return fmt.Errorf("failed to serve: %v", err)
	}

	return nil
}

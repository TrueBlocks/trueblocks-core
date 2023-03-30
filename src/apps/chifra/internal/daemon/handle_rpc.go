package daemonPkg

import (
	"context"
	"fmt"
	"net"
	"os"

	pb "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/grpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"google.golang.org/grpc"
)

const sockPath = "/tmp/trueblocks.sock"

type grpcServer struct {
	pb.UnimplementedNamesServer
}

// Search looks up name by given terms
func (g *grpcServer) Search(ctx context.Context, request *pb.SearchRequest) (*pb.SearchResponse, error) {
	logger.Info("Handling SearchNames")
	found, err := names.LoadNamesArray("mainnet", names.Parts(request.GetParts()), names.SortByAddress, request.GetTerms())
	if err != nil {
		return nil, err
	}
	pnames := make([]*pb.Name, 0, len(found))

	for _, name := range found {
		pnames = append(pnames, name.ToMessage())
	}

	return &pb.SearchResponse{
		Names: pnames,
	}, nil
}

// SearchStream is like Search, but it streams the response
func (g *grpcServer) SearchStream(request *pb.SearchRequest, stream pb.Names_SearchStreamServer) error {
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
	if err := os.RemoveAll(sockPath); err != nil {
		return err
	}

	// TODO(names): temp
	_, err := names.LoadNamesMap("mainnet", names.Regular|names.Custom, nil)
	if err != nil {
		return err
	}

	// r := new(Rpc)
	// rpc.Register(r)
	r := grpc.NewServer()
	pb.RegisterNamesServer(r, &grpcServer{})

	listener, err := net.Listen("unix", sockPath)
	if err != nil {
		return err
	}

	if err := r.Serve(listener); err != nil {
		return fmt.Errorf("failed to serve: %v", err)
	}

	return nil
}

package daemonPkg

import (
	"context"
	"fmt"
	"net"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/proto"
	"google.golang.org/grpc"
)

type chifraRpcServer struct {
	proto.UnimplementedNamesServer
}

// Search looks up name by given terms
func (g *chifraRpcServer) Search(ctx context.Context, request *proto.SearchRequest) (*proto.SearchResponse, error) {
	log("Handling SearchNames")
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
	log("Handling SearchStream")
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

// CRUD
func (g *chifraRpcServer) Create(ctx context.Context, request *proto.CreateRequest) (*proto.CRUDResponse, error) {
	log("Handling Create")
	if request.Chain == "test" {
		return &proto.CRUDResponse{
			Success: true,
			Error:   utils.PointerOf(""),
		}, nil
	}

	name := types.NewNameFromGrpc(request.GetName())
	err := names.CreateCustomName(request.GetChain(), name)
	var errPointer *string
	if err != nil {
		errPointer = utils.PointerOf(err.Error())
	}

	return &proto.CRUDResponse{
		Success: err == nil,
		Error:   errPointer,
	}, err
}

func (g *chifraRpcServer) Update(ctx context.Context, request *proto.CreateRequest) (*proto.CRUDResponse, error) {
	log("Handling Update")
	return g.Create(ctx, request)
}

func (g *chifraRpcServer) Delete(ctx context.Context, request *proto.DeleteRequest) (*proto.CRUDResponse, error) {
	log("Handling Delete")
	return handleNameDeletion(request, true, false)
}

func (g *chifraRpcServer) Undelete(ctx context.Context, request *proto.DeleteRequest) (*proto.CRUDResponse, error) {
	log("Handling Undelete")
	return handleNameDeletion(request, false, false)
}

func (g *chifraRpcServer) Remove(ctx context.Context, request *proto.DeleteRequest) (*proto.CRUDResponse, error) {
	log("Handling Remove")
	return handleNameDeletion(request, true, true)
}

func handleNameDeletion(request *proto.DeleteRequest, deleted bool, remove bool) (*proto.CRUDResponse, error) {
	addr := base.HexToAddress(request.Address)
	var err error
	if !remove {
		_, err = names.ChangeCustomNameDeletedFlag(request.Chain, addr, deleted)
	} else {
		_, err = names.RemoveCustomName(request.Chain, addr)
	}
	return &proto.CRUDResponse{
		Success: err == nil,
		Error:   utils.PointerOf(err.Error()),
	}, err
}

func (opts *DaemonOptions) HandleGrpc() error {
	if !opts.Grpc {
		return nil
	}
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

	log("Server started")
	if err := rpcServer.Serve(listener); err != nil {
		return fmt.Errorf("failed to serve: %v", err)
	}

	return nil
}

func log(v ...any) {
	logger.Info("gRPC: " + fmt.Sprint(v...))
}

package output

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type RenderCtx struct {
	Ctx          context.Context    `json:"-"`
	Cancel       context.CancelFunc `json:"-"`
	ModelChan    chan types.Modeler `json:"-"`
	ErrorChan    chan error         `json:"-"`
	ProgressChan chan int           `json:"-"` // place holder
}

func NewRenderContext() *RenderCtx {
	ctx, cancel := context.WithCancel(context.Background())
	return &RenderCtx{
		Ctx:    ctx,
		Cancel: cancel,
	}
}

func WithChannels() *RenderCtx {
	rCtx := NewRenderContext()
	// TODO: Should these be buffered channels? Issue #3821
	rCtx.ModelChan = make(chan types.Modeler)
	rCtx.ErrorChan = make(chan error)
	rCtx.ProgressChan = make(chan int)
	return rCtx
}

func (r *RenderCtx) WasCanceled() bool {
	select {
	case <-r.Ctx.Done():
		return true
	default:
		return false
	}
}

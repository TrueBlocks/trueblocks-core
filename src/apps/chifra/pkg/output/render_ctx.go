package output

import "context"

type RenderCtx struct {
	Ctx    context.Context    `json:"-"`
	Cancel context.CancelFunc `json:"-"`
	// TODO: Add a place for references to caller-level channels
	// TODO: for progress, errors, and data
}

func NewRenderContext() RenderCtx {
	ctx, cancel := context.WithCancel(context.Background())
	return RenderCtx{
		Ctx:    ctx,
		Cancel: cancel,
	}
}

func (r *RenderCtx) ShouldQuit() bool {
	select {
	case <-r.Ctx.Done():
		return true
	default:
		return false
	}
}

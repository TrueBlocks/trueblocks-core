package charts

import (
	"image/color"
	"math"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"gonum.org/v1/plot"
	"gonum.org/v1/plot/vg"
	"gonum.org/v1/plot/vg/draw"
)

type Labels struct {
	X     string
	Y     string
	Title string
}

func getPlot(labels Labels) (*plot.Plot, error) {
	p, err := plot.New()
	if err != nil {
		logger.Panic(err)
	}

	p.Title.Font.Size = vg.Points(18)
	p.Title.Padding = vg.Points(10)
	p.Title.Text = labels.Title

	p.BackgroundColor = bgColor

	p.X.Label.Text = labels.X
	p.X.LineStyle.Color = color.Black
	p.X.Padding = vg.Points(10)
	p.X.Tick.Color = color.Black
	p.X.Tick.Label.Color = color.Black
	p.X.Tick.Label.Rotation = math.Pi / 2.8
	p.X.Tick.Label.XAlign = draw.XRight
	p.X.Tick.Label.YAlign = draw.YBottom

	p.Y.Label.Text = labels.Y
	p.Y.LineStyle.Color = color.Black
	p.Y.Padding = vg.Points(10)
	p.Y.Tick.Color = color.Black
	p.Y.Tick.Label.Color = color.Black

	return p, nil
}

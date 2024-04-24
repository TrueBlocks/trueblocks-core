package charts

import (
	"image/color"

	"gonum.org/v1/plot"
	"gonum.org/v1/plot/vg"
)

func applyCustomStyling(p *plot.Plot, title string) {
	p.Title.Text = title
	p.Title.Font.Size = vg.Points(25)
	p.Title.Color = color.RGBA{R: 255, G: 255, B: 255, A: 255}
	p.X.Label.Color = color.RGBA{R: 255, G: 255, B: 255, A: 255}
	p.Y.Label.Color = color.RGBA{R: 255, G: 255, B: 255, A: 255}
	p.X.Color = color.RGBA{R: 255, G: 255, B: 255, A: 255}
	p.Y.Color = color.RGBA{R: 255, G: 255, B: 255, A: 255}
	p.X.Label.Text = "Timestamp"
	p.Y.Label.Text = "Balance"
	p.BackgroundColor = color.RGBA{R: 128, G: 128, B: 128, A: 255}
	p.Legend.Color = color.RGBA{R: 255, G: 255, B: 255, A: 255}
	p.Legend.Font.SetName("Courier New")
	p.Legend.Left = true
	p.Legend.Top = true
}

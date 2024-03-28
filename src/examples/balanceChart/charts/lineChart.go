package charts

import (
	"image/color"
	"log"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"gonum.org/v1/plot"
	"gonum.org/v1/plot/plotter"
	"gonum.org/v1/plot/vg"
)

func LineChart(data []types.SimpleState, title string, filename string) {
	// Manually define a color palette
	colorPalette := []color.Color{
		color.RGBA{R: 255, G: 0, B: 0, A: 255},   // Red
		color.RGBA{R: 0, G: 255, B: 0, A: 255},   // Green
		color.RGBA{R: 0, G: 0, B: 255, A: 255},   // Blue
		color.RGBA{R: 255, G: 255, B: 0, A: 255}, // Yellow
		// Add more colors as needed
	}

	// Group data by address
	groupedByAddress := make(map[string][]types.SimpleState)
	for _, d := range data {
		groupedByAddress[d.Address.Hex()] = append(groupedByAddress[d.Address.Hex()], d)
	}

	// Create a new plot
	p, err := plot.New()
	if err != nil {
		log.Fatalf("Error creating plot: %v", err)
	}

	applyCustomStyling(p, title)

	colorIndex := 0

	// Iterate over each group and create a line plot
	for address, group := range groupedByAddress {
		points := make(plotter.XYs, len(group))
		for i, d := range group {
			points[i].X = float64(d.BlockNumber)
			points[i].Y = d.Balance.Float64()
		}

		line, err := plotter.NewLine(points)
		if err != nil {
			log.Fatalf("Error creating line plot for address %s: %v", address, err)
		}

		// Set the color for the line
		line.Color = colorPalette[colorIndex%len(colorPalette)]
		line.Width = vg.Points(2)

		// Add the line plot to the plot
		p.Add(line)

		// Add a legend entry for this line
		p.Legend.Add(address, line)

		// Move to the next color for the next address
		colorIndex++
	}

	// Save the plot to an SVG file
	if err := p.Save(16*vg.Inch, 8*vg.Inch, filename); err != nil {
		log.Fatalf("Error saving plot: %v", err)
	}
}

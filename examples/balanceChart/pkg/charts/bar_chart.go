package charts

import (
	"fmt"
	"log"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"gonum.org/v1/plot/plotter"
	"gonum.org/v1/plot/vg"
	"gonum.org/v1/plot/vg/draw"
	"gonum.org/v1/plot/vg/vgimg"
)

var barWidth = 5.
var borderWidth = 1.

// BarChart creates a grouped bar chart from an array of State objects
func BarChart(names []types.Name, balances []types.State, labels Labels, fileName string) {
	// Assign an ID to each address. We will use this to keep the ordering of
	// the bars consistent
	id := 1 // because 0 is the default value for an int
	idMap := make(map[base.Address]int)
	for _, d := range balances {
		if idMap[d.Address] == 0 {
			idMap[d.Address] = id
			id++
		}
	}
	nAddrs := len(idMap)

	// Create a map of addresses to names which gives us a way to look up the name of an address
	namesMap := make(map[base.Address]types.Name, len(names))
	for _, n := range names {
		namesMap[n.Address] = n
	}

	// For each address, we create an offest within the group for that address's bar
	offsets := []float64{}
	for i := 0; i < nAddrs; i++ {
		// offset is the distance from the center of the group
		// to the center of the bar for that address
		offset := float64(i)*barWidth - float64(i)*(barWidth/2.0)
		fmt.Printf("offset for address %d: %f\n", i, offset)
		offsets = append(offsets, offset)
	}

	// For each address, we create a history of the balances for that address
	histories := make([]plotter.Values, nAddrs) // a slice of slices of floats
	for _, bal := range balances {
		idx := idMap[bal.Address] - 1
		ff := base.ToEther(&bal.Balance).Float64()
		histories[idx] = append(histories[idx], ff)
	}

	// We're finally ready to create the plot
	p, err := getPlot(labels)
	if err != nil {
		logger.Panic(err)
	}

	totalWidth := 0
	// For each address, we create a bar, offset for that address, of the correct height.
	for i := 0; i < nAddrs; i++ {
		bar, err := plotter.NewBarChart(histories[i], vg.Points(barWidth))
		if err != nil {
			log.Panic(err)
		}
		bar.Color = colors[i%len(colors)]
		bar.LineStyle.Color = darkenColor(bar.Color, 0.8)
		bar.LineStyle.Width = vg.Length(borderWidth)
		bar.Offset = vg.Points(offsets[i])
		totalWidth += nAddrs * int(barWidth)
		p.Add(bar)
	}

	// nTicks := 6
	// lWid := totalWidth / (nTicks + 1)
	// ticks := []plot.Tick{}
	// for i := 0; i < nTicks; i++ {
	// 	v := i * lWid
	// 	ticks = append(ticks, plot.Tick{Value: float64(v), Label: fmt.Sprintf("%d", v)})
	// }
	// p.X.Tick.Marker = plot.ConstantTicks(ticks)

	filename := file.ValidFilename(labels.Title, true) + ".png"
	if len(fileName) > 0 {
		filename = fileName
	}

	// Save the plot to a PNG file with added padding
	const width, height = 10 * vg.Inch, 6 * vg.Inch
	img := vgimg.New(width+vg.Points(10), height)
	dc := draw.New(img)
	dc = draw.Crop(dc, 0, -vg.Points(10), 0, 0)
	p.Draw(dc)

	w, err := os.Create(filename)
	if err != nil {
		log.Panic(err)
	}
	defer w.Close()

	png := vgimg.PngCanvas{Canvas: img}
	if _, err := png.WriteTo(w); err != nil {
		log.Panic(err)
	}

	utils.System("open " + filename)
}

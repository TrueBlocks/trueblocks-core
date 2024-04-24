package charts

import (
	"fmt"
	"image/color"
	"log"
	"math"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"gonum.org/v1/plot"
	"gonum.org/v1/plot/plotter"
	"gonum.org/v1/plot/vg"
	"gonum.org/v1/plot/vg/draw"
)

// GroupedBarChart creates a grouped bar chart from an array of State objects
func GroupedBarChart(data []types.State, names []types.Name, title string, filename string) {
	var err error
	var p *plot.Plot
	if p, err = plot.New(); err != nil {
		log.Panic(err)
	}

	p.Title.Text = title
	p.Title.Font.Size = vg.Points(18)
	p.X.Label.Text = "Block Number"
	p.Y.Label.Text = "Balance"
	p.BackgroundColor = bgColor
	// p.Legend.Top = true
	// p.Legend.Left = true
	// p.Legend.Font.Size = vg.Points(11)
	// p.Legend.Font.SetName("Mono")
	p.Title.Padding = vg.Points(10)
	p.X.Padding = vg.Points(10)
	p.Y.Padding = vg.Points(10)
	p.X.Tick.Label.Rotation = math.Pi / 2.8
	p.X.Tick.Label.XAlign = draw.XRight
	p.X.Tick.Label.YAlign = draw.YBottom
	p.X.Tick.Label.Color = color.Black
	p.X.Tick.Color = color.Black
	p.X.LineStyle.Color = color.Black
	p.Y.Tick.Label.Color = color.Black
	p.Y.Tick.Color = color.Black
	p.Y.LineStyle.Color = color.Black

	namesMap := make(map[string]types.Name, len(names))
	for _, n := range names {
		namesMap[small(n.Address)] = n
	}

	addrMap := make(map[string]int)
	cnt := 1
	for _, d := range data {
		if addrMap[small(d.Address)] == 0 {
			addrMap[small(d.Address)] = cnt
			cnt++
		}
	}

	addrs := []string{}
	for key := range addrMap {
		addrs = append(addrs, key)
	}
	nAddrs := len(addrs)

	wid := 4.

	offsets := []float64{}
	for i := 0; i < nAddrs; i++ {
		f := float64(-4 + (i * 4))
		fmt.Println(i, i+1, -4+(i*4), f)
		offsets = append(offsets, f) // []float64{-4, -2, 2, 4}
	}
	// offsetx := []float64{ 0,  2, 6, 8}

	balances := []plotter.Values{}
	for i := 0; i < nAddrs; i++ {
		balances = append(balances, plotter.Values{})
	}

	for _, d := range data {
		idx := addrMap[small(d.Address)] - 1
		ff := base.ToEther(&d.Balance).Float64()
		balances[idx] = append(balances[idx], ff)
	}
	for i := 0; i < nAddrs; i++ {
		balances[i] = append(balances[i], 0)
	}

	if len(balances) != nAddrs {
		log.Panic("Invalid balance length. Have ", len(balances), " want ", nAddrs)
	}
	if len(offsets) < nAddrs {
		log.Panic("Invalid offsets length. Have ", len(offsets), " want ", nAddrs)
	}

	minOffset := 0
	maxOffset := 0
	for i := 0; i < nAddrs; i++ {
		bar, err := plotter.NewBarChart(balances[i], vg.Points(wid))
		if err != nil {
			log.Panic(err)
		}
		bar.LineStyle.Width = vg.Length(1)
		bar.LineStyle.Color = colors[(i+6)%len(colors)]
		bar.Color = colors[i%len(colors)]
		bar.Offset = vg.Points(offsets[i])
		maxOffset += nAddrs * int(wid)
		fmt.Println("Adding", namesMap[addrs[i]].Name, "at", offsets[i], "for", addrs[i])
		// p.Legend.Add(namesMap[addrs[i]].Name, bar)
		p.Add(bar)
	}

	nTicks := 6
	lWid := (maxOffset - minOffset) / nTicks
	ticks := []plot.Tick{}
	for i := 0; i < nTicks; i++ {
		v := minOffset + i*lWid
		ticks = append(ticks, plot.Tick{Value: float64(v), Label: fmt.Sprintf("%d", v)})
	}
	p.X.Tick.Marker = plot.ConstantTicks(ticks)

	if err := p.Save(16*vg.Inch, 8*vg.Inch, filename); err != nil {
		log.Panic(err)
	}

	utils.System("open " + filename)
}

var bgColor = color.RGBA{R: 255, G: 255, B: 255, A: 255}
var colors = []color.Color{
	color.RGBA{0x00, 0x00, 0xff, 0xff},
	color.RGBA{0x00, 0xff, 0x00, 0xff},
	color.RGBA{0xff, 0x00, 0x00, 0xff},
	color.RGBA{0x00, 0xff, 0xff, 0xff},
	color.RGBA{0xff, 0xff, 0x00, 0xff},
	color.RGBA{0xff, 0x00, 0xff, 0xff},

	color.RGBA{0x00, 0x00, 0x88, 0xff},
	color.RGBA{0x00, 0x88, 0x00, 0xff},
	color.RGBA{0x88, 0x00, 0x00, 0xff},
	color.RGBA{0x00, 0x88, 0x88, 0xff},
	color.RGBA{0x88, 0x88, 0x00, 0xff},
	color.RGBA{0x88, 0x00, 0x88, 0xff},
}

func small(a base.Address) string {
	return a.Hex()[:6] + "..." + a.Hex()[len(a.Hex())-4:]
}

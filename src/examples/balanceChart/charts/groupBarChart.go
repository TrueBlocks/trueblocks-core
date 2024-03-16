package charts

import (
	"image/color"
	"log"
	"sort"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"gonum.org/v1/plot"
	"gonum.org/v1/plot/plotter"
	"gonum.org/v1/plot/vg"
)

func GroupedBarChart(data []types.SimpleState, filename string) {
	// Manually define a color palette
	colorPalette := []color.Color{
		color.RGBA{R: 255, G: 0, B: 0, A: 255},   // Red
		color.RGBA{R: 0, G: 255, B: 0, A: 255},   // Green
		color.RGBA{R: 0, G: 0, B: 255, A: 255},   // Blue
		color.RGBA{R: 255, G: 255, B: 0, A: 255}, // Yellow
	}

	// Create a new plot
	p, err := plot.New()
	if err != nil {
		log.Fatalf("Error creating plot: %v", err)
	}

	applyCustomStyling(p)

	// Extract unique block numbers and sort them
	blockNumbers := uniqueBlockNumbers(data)
	sortedBlocks := sort.IntSlice(blockNumbers)
	sort.Sort(sortedBlocks)

	w := vg.Points(4)
	offsets := calculateOffsets(len(data), len(sortedBlocks), w)

	groupedByAddress := groupDataByAddress(data)

	colorIndex := 0
	for address, group := range groupedByAddress {
		bars, err := createBarsForGroup(group, sortedBlocks, w, offsets[colorIndex], colorPalette[colorIndex%len(colorPalette)])
		if err != nil {
			log.Fatalf("Error creating bars for address %s: %v", address, err)
		}
		p.Add(bars)
		p.Legend.Add(address, bars)

		colorIndex++
	}

	// Customizing the X-axis
	p.NominalX(blockNumberLabels(sortedBlocks)...)

	// Save the plot to an SVG file
	if err := p.Save(16*vg.Inch, 8*vg.Inch, filename); err != nil {
		log.Fatalf("Error saving plot: %v", err)
	}
}

func blockNumberLabels(sortedBlocks []int) []string {
	labels := make([]string, len(sortedBlocks))
	for i, block := range sortedBlocks {
		labels[i] = strconv.Itoa(block)
	}
	return labels
}

func createBarsForGroup(group []types.SimpleState, sortedBlocks []int, barWidth vg.Length, offset vg.Length, color color.Color) (*plotter.BarChart, error) {
	values := make(plotter.Values, len(sortedBlocks))
	blockIndexMap := make(map[int]int)
	for i, block := range sortedBlocks {
		blockIndexMap[block] = i
	}

	for _, state := range group {
		if index, exists := blockIndexMap[int(state.BlockNumber)]; exists {
			value, err := strconv.ParseFloat(utils.FormattedValue(state.Balance, true, 18), 64)
			if err != nil {
				return nil, err
			}
			values[index] = value
		}
	}

	bars, err := plotter.NewBarChart(values, barWidth)
	if err != nil {
		return nil, err
	}
	bars.Color = color
	bars.Offset = offset
	return bars, nil
}

// groupDataByAddress groups the input data by the Address field.
func groupDataByAddress(data []types.SimpleState) map[string][]types.SimpleState {
	grouped := make(map[string][]types.SimpleState)
	for _, item := range data {
		addressHex := item.Address.Hex()
		grouped[addressHex] = append(grouped[addressHex], item)
	}
	return grouped
}

// Updated calculateOffsets function signature to include numBlocks
func calculateOffsets(numGroups, numBlocks int, barWidth vg.Length) []vg.Length {
	offsets := make([]vg.Length, numGroups)
	gap := barWidth * 0.2
	groupWidth := barWidth + gap
	totalWidth := vg.Length(numBlocks) * groupWidth
	startOffset := -totalWidth/2 + barWidth/2
	for i := 0; i < numGroups; i++ {
		offsets[i] = startOffset + vg.Length(i)*groupWidth
	}
	return offsets
}

func uniqueBlockNumbers(data []types.SimpleState) []int {
	blockMap := make(map[int]bool)
	for _, item := range data {
		blockMap[int(item.BlockNumber)] = true
	}
	uniqueBlocks := make([]int, 0, len(blockMap))
	for blockNumber := range blockMap {
		uniqueBlocks = append(uniqueBlocks, blockNumber)
	}

	return uniqueBlocks
}

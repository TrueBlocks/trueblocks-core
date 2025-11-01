package manifest

import (
	"encoding/json"
	"fmt"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

// SaveManifest writes the manifest to a file in JSON format. It first removes any
// duplicate chunks and sorts them. It then marshals the manifest into JSON and
// writes it to the file. The function takes the chain name and file name as arguments.
func (m *Manifest) SaveManifest(chain, fileName string) error {
	m.Config = config.GetScrape(chain)
	m.Config.ChannelCount = 0 // Exclude ChannelCount from the JSON

	w, err := os.OpenFile(fileName, os.O_RDWR|os.O_CREATE|os.O_TRUNC, 0644)
	if err != nil {
		return fmt.Errorf("creating file: %s", err)
	}
	defer func() {
		w.Close()
		config.SetExpectedVersion(m.Version)
	}()

	err = file.Lock(w)
	if err != nil {
		return fmt.Errorf("locking file: %s", err)
	}
	defer func() {
		_ = file.Unlock(w)
	}()

	m.removeDuplicatesAndSort()

	outputBytes, err := json.MarshalIndent(m, "", "  ")
	if err != nil {
		return err
	}

	_, err = w.Write(outputBytes)
	return err
}

// removeDuplicatesAndSort is a helper function that removes duplicate chunks from the manifest
// and sorts the remaining chunks by their range. It uses a map to track seen ranges and a slice
// to store unique chunks. It then sorts the slice and updates the manifest's chunks.
func (m *Manifest) removeDuplicatesAndSort() {
	seen := make(map[string]bool)
	chunks := make([]types.ChunkRecord, 0, len(m.Chunks))

	for i := len(m.Chunks) - 1; i >= 0; i-- {
		if !seen[m.Chunks[i].Range] {
			seen[m.Chunks[i].Range] = true
			chunks = append(chunks, m.Chunks[i])
		}
	}

	sort.Slice(chunks, func(i, j int) bool {
		return chunks[i].Range < chunks[j].Range
	})

	m.Chunks = chunks
}

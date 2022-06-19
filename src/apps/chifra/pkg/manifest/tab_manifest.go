// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package manifest

import (
	"encoding/csv"
	"io"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// ReadPinDescriptors parses pin information and returns a slice of
// Chunk or error
func ReadPinDescriptors(r io.Reader) ([]Chunk, error) {
	reader := csv.NewReader(r)
	reader.Comma = '\t'
	reader.FieldsPerRecord = 3

	descriptors := []Chunk{}

	for {
		record, err := reader.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			return nil, err
		}
		if record[0] == "fileName" {
			continue
		}

		descriptors = append(descriptors, Chunk{
			FileName:  record[0],
			BloomHash: record[1],
			IndexHash: record[2],
		})
	}

	return descriptors, nil
}

// ReadTabManifest parses TSV formatted manifest
func ReadTabManifest(chain string, r io.Reader) (*Manifest, error) {
	descriptors, err := ReadPinDescriptors(r)
	if err != nil {
		return nil, err
	}

	return &Manifest{
		Version:   version.ManifestVersion,
		Chain:     chain,
		Schemas:   unchained.Schemas,
		Databases: unchained.Databases,
		Chunks:    descriptors,
	}, nil
}

// FromCache loads the manifest saved in ConfigPath
func FromCache(chain string) (*Manifest, error) {
	manifestPath := config.GetPathToChainConfig(chain) + "manifest.txt"
	file, err := os.Open(manifestPath)
	if err != nil {
		return nil, err
	}
	return ReadTabManifest(chain, file)
}

func GetChunkList(chain string) ([]types.SimpleChunk, error) {
	manFromCache, err := FromCache(chain)
	if err != nil {
		return []types.SimpleChunk{}, err
	}

	sort.Slice(manFromCache.Chunks, func(i, j int) bool {
		iPin := manFromCache.Chunks[i]
		jPin := manFromCache.Chunks[j]
		return iPin.FileName < jPin.FileName
	})

	pinList := make([]types.SimpleChunk, len(manFromCache.Chunks))
	for i := range manFromCache.Chunks {
		pinList[i].FileName = manFromCache.Chunks[i].FileName
		pinList[i].BloomHash = string(manFromCache.Chunks[i].BloomHash)
		pinList[i].IndexHash = string(manFromCache.Chunks[i].IndexHash)
	}
	return pinList, nil
}

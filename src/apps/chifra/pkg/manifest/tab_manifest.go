// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package manifest

import (
	"encoding/csv"
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// readPinDescriptors parses pin information and returns a slice of
// ChunkRecord or error
func readPinDescriptors(r io.Reader) ([]ChunkRecord, error) {
	reader := csv.NewReader(r)
	reader.Comma = '\t'
	reader.FieldsPerRecord = 3

	descriptors := []ChunkRecord{}

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

		descriptors = append(descriptors, ChunkRecord{
			Range:     record[0],
			BloomHash: types.IpfsHash(record[1]),
			IndexHash: types.IpfsHash(record[2]),
		})
	}

	return descriptors, nil
}

// readTabManifest parses TSV formatted manifest
func readTabManifest(chain string, r io.Reader) (*Manifest, error) {
	descriptors, err := readPinDescriptors(r)
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

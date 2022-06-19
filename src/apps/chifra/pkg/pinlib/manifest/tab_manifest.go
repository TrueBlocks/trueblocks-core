// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package manifest

import (
	"encoding/csv"
	"errors"
	"io"
	"os"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// ReadPinDescriptors parses pin information and returns a slice of
// PinDescriptor or error
func ReadPinDescriptors(r io.Reader) ([]PinDescriptor, error) {
	reader := csv.NewReader(r)
	reader.Comma = '\t'
	reader.FieldsPerRecord = 3

	descriptors := []PinDescriptor{}

	for {
		record, err := reader.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			return nil, err
		}

		descriptors = append(descriptors, PinDescriptor{
			FileName:  record[0],
			BloomHash: record[1],
			IndexHash: record[2],
		})
	}

	return descriptors, nil
}

// BuildTabRange finds the first and last pin descriptor and returns
// ManifestRange. We need this function, because TSV formatted manifest
// does not carry this information explicitly
func BuildTabRange(descriptors []PinDescriptor) (ManifestRange, error) {
	if len(descriptors) == 0 {
		return ManifestRange{}, errors.New("invalid input: no pins")
	}

	firstPinRange := ManifestRange{}
	var err error
	if !strings.Contains(descriptors[0].FileName, "-") {
		if len(descriptors) == 1 {
			return ManifestRange{}, errors.New("invalid input: header only, no pins")
		}
		firstPinRange, err = StringToManifestRange(descriptors[1].FileName)
		if err != nil {
			return ManifestRange{}, err
		}
	} else {
		firstPinRange, err = StringToManifestRange(descriptors[0].FileName)
		if err != nil {
			return ManifestRange{}, err
		}
	}

	lastPinRange, err := StringToManifestRange(descriptors[len(descriptors)-1].FileName)
	if err != nil {
		return ManifestRange{}, err
	}

	return ManifestRange{
		firstPinRange[0],
		lastPinRange[1],
	}, nil
}

// ReadTabManifest parses TSV formatted manifest
func ReadTabManifest(r io.Reader) (*Manifest, error) {
	descriptors, err := ReadPinDescriptors(r)
	if err != nil {
		return nil, err
	}

	newBlockRange, err := BuildTabRange(descriptors)
	if err != nil {
		return nil, err
	}

	return &Manifest{
		IndexFormat: "",
		BloomFormat: "",
		CommitHash:  "",
		BlockRange:  newBlockRange,
		Pins:        descriptors,
	}, nil
}

// FromLocalFile loads the manifest saved in ConfigPath
func FromLocalFile(chain string) (*Manifest, error) {
	manifestPath := config.GetPathToChainConfig(chain) + "manifest.txt"
	file, err := os.Open(manifestPath)
	if err != nil {
		return nil, err
	}
	return ReadTabManifest(file)
}

func GetPinList(chain string) ([]types.SimplePinList, error) {
	manifestData, err := FromLocalFile(chain)
	if err != nil {
		return []types.SimplePinList{}, err
	}

	sort.Slice(manifestData.Pins, func(i, j int) bool {
		iPin := manifestData.Pins[i]
		jPin := manifestData.Pins[j]
		return iPin.FileName < jPin.FileName
	})

	pinList := make([]types.SimplePinList, len(manifestData.Pins))
	for i := range manifestData.Pins {
		pinList[i].FileName = manifestData.Pins[i].FileName
		pinList[i].BloomHash = string(manifestData.Pins[i].BloomHash)
		pinList[i].IndexHash = string(manifestData.Pins[i].IndexHash)
	}
	return pinList, nil
}

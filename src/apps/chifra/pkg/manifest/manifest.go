// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package manifest

import "encoding/json"

type IpfsHash = string

type ChunkRecord struct {
	FileName  string   `json:"fileName"`
	BloomHash IpfsHash `json:"bloomHash"`
	IndexHash IpfsHash `json:"indexHash"`
	// BloomChecksum string   `json:"bloomChecksum"`
	// IndexChecksum string   `json:"indexChecksum"`
	// file size (gzipped file)
	// md5
}

type Manifest struct {
	Version     json.Number   `json:"version"`
	Chain       string        `json:"chain"`
	ChainId     json.Number   `json:"chainId"` // not sure
	IndexFormat IpfsHash      `json:"indexFormat"`
	BloomFormat IpfsHash      `json:"bloomFormat"`
	CommitHash  string        `json:"commitHash"`
	BlockRange  ManifestRange `json:"blockRange"`
	Names       IpfsHash      `json:"names"`
	Timestamps  IpfsHash      `json:"timestamps"`
	Chunks      []ChunkRecord `json:"chunks"`
}

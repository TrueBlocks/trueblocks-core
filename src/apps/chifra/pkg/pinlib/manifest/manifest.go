// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package manifest

type IpfsHash = string

// TODO: If we included fileSize (pre-zip) here, we could do a check that the file is complete
type Chunk struct {
	FileName  string   `json:"fileName"`
	BloomHash IpfsHash `json:"bloomHash"`
	IndexHash IpfsHash `json:"indexHash"`
}

type Manifest struct {
	Version   string   `json:"version"`
	Chain     string   `json:"chain"`
	Schemas   IpfsHash `json:"schemas"`
	Databases IpfsHash `json:"databases"`
	Chunks    []Chunk  `json:"chunks"`
}

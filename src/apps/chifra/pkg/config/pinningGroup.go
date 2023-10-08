// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"strings"

	shell "github.com/ipfs/go-ipfs-api"
)

type pinningGroup struct {
	GatewayURL   string `toml:"gatewayUrl"`
	LocalPinUrl  string `toml:"localPinUrl"`
	RemotePinUrl string `toml:"remotePinUrl"`
}

// func GetPinning() pinningGroup {
// 	return GetRootConfig().Pinning
// }

func IpfsRunning() bool {
	sh := shell.NewShell(IPFS_URL)
	_, err := sh.Add(strings.NewReader("hello world!"))
	return err == nil
}

// TODO: should be configurable (see #2804)
const (
	PINATA_URL = "https://api.pinata.cloud/pinning/pinFileToIPFS"
	IPFS_URL   = "http://localhost:5001"
)

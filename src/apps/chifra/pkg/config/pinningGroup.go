// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"strings"

	shell "github.com/ipfs/go-ipfs-api"
)

const defaultIpfsGateway = "https://ipfs.unchainedindex.io/ipfs/"

type pinningGroup struct {
	GatewayUrl   string `toml:"gatewayUrl" comment:"The pinning gateway to query when downloading the unchained index"`
	LocalPinUrl  string `toml:"localPinUrl" comment:"The local endpoint for the IPFS daemon"`
	RemotePinUrl string `toml:"remotePinUrl" comment:"The remote endpoint for pinning on Pinata"`
}

func GetPinning() pinningGroup {
	return GetRootConfig().Pinning
}

func IpfsRunning() bool {
	sh := shell.NewShell(GetPinning().LocalPinUrl)
	_, err := sh.Add(strings.NewReader("hello world!"))
	return err == nil
}

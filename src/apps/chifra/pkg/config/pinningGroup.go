// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/configtypes"
	shell "github.com/ipfs/go-ipfs-api"
)

const defaultIpfsGateway = "https://ipfs.unchainedindex.io/ipfs/"

func GetPinning() configtypes.PinningGroup {
	return GetRootConfig().Pinning
}

func IpfsRunning() bool {
	sh := shell.NewShell(GetPinning().LocalPinUrl)
	_, err := sh.Add(strings.NewReader("hello world!"))
	return err == nil
}

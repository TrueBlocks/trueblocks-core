// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
from . import session

exportCmd = "export"
exportPos = "addrs"
exportFmt = "json"
exportOpts = {
    "appearances": {"hotkey": "-p", "type": "switch"},
    "receipts": {"hotkey": "-r", "type": "switch"},
    "logs": {"hotkey": "-l", "type": "switch"},
    "traces": {"hotkey": "-t", "type": "switch"},
    "neighbors": {"hotkey": "-n", "type": "switch"},
    "accounting": {"hotkey": "-C", "type": "switch"},
    "statements": {"hotkey": "-A", "type": "switch"},
    "balances": {"hotkey": "-b", "type": "switch"},
    "withdrawals": {"hotkey": "-i", "type": "switch"},
    "articulate": {"hotkey": "-a", "type": "switch"},
    "cacheTraces": {"hotkey": "-R", "type": "switch"},
    "count": {"hotkey": "-U", "type": "switch"},
    "firstRecord": {"hotkey": "-c", "type": "flag"},
    "maxRecords": {"hotkey": "-e", "type": "flag"},
    "relevant": {"hotkey": "-N", "type": "switch"},
    "emitter": {"hotkey": "-m", "type": "flag"},
    "topic": {"hotkey": "-B", "type": "flag"},
    "reverted": {"hotkey": "-V", "type": "switch"},
    "asset": {"hotkey": "-P", "type": "flag"},
    "flow": {"hotkey": "-f", "type": "flag"},
    "factory": {"hotkey": "-y", "type": "switch"},
    "unripe": {"hotkey": "-u", "type": "switch"},
    "reversed": {"hotkey": "-E", "type": "switch"},
    "noZero": {"hotkey": "-z", "type": "switch"},
    "firstBlock": {"hotkey": "-F", "type": "flag"},
    "lastBlock": {"hotkey": "-L", "type": "flag"},
    "chain": {"hotkey": "", "type": "flag"},
    "noHeader": {"hotkey": "", "type": "switch"},
    "cache": {"hotkey": "-o", "type": "switch"},
    "decache": {"hotkey": "-D", "type": "switch"},
    "ether": {"hotkey": "-H", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
}

def export(self):
    ret = self.toUrl(exportCmd, exportPos, exportFmt, exportOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text

// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
from . import session

listCmd = "list"
listPos = "addrs"
listFmt = "json"
listOpts = {
    "count": {"hotkey": "-U", "type": "switch"},
    "noZero": {"hotkey": "-z", "type": "switch"},
    "bounds": {"hotkey": "-b", "type": "switch"},
    "unripe": {"hotkey": "-u", "type": "switch"},
    "silent": {"hotkey": "-s", "type": "switch"},
    "firstRecord": {"hotkey": "-c", "type": "flag"},
    "maxRecords": {"hotkey": "-e", "type": "flag"},
    "reversed": {"hotkey": "-E", "type": "switch"},
    "firstBlock": {"hotkey": "-F", "type": "flag"},
    "lastBlock": {"hotkey": "-L", "type": "flag"},
    "chain": {"hotkey": "", "type": "flag"},
    "noHeader": {"hotkey": "", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
}

def list(self):
    ret = self.toUrl(listCmd, listPos, listFmt, listOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text

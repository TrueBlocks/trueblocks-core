// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
from . import session

monitorsCmd = "monitors"
monitorsPos = "addrs"
monitorsFmt = "json"
monitorsOpts = {
    "delete": {"hotkey": "", "type": "switch"},
    "undelete": {"hotkey": "", "type": "switch"},
    "remove": {"hotkey": "", "type": "switch"},
    "clean": {"hotkey": "-C", "type": "switch"},
    "list": {"hotkey": "-l", "type": "switch"},
    "watch": {"hotkey": "-w", "type": "switch"},
    "watchlist": {"hotkey": "-a", "type": "flag"},
    "commands": {"hotkey": "-c", "type": "flag"},
    "batchSize": {"hotkey": "-b", "type": "flag"},
    "sleep": {"hotkey": "-s", "type": "flag"},
    "chain": {"hotkey": "-", "type": "flag"},
    "noHeader": {"hotkey": "-", "type": "switch"},
    "cache": {"hotkey": "-o", "type": "switch"},
    "decache": {"hotkey": "-D", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
}

def monitors(self):
    ret = self.toUrl(monitorsCmd, monitorsPos, monitorsFmt, monitorsOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text

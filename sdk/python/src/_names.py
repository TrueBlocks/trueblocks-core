// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
from . import session

namesCmd = "names"
namesPos = "terms"
namesFmt = "json"
namesOpts = {
    "expand": {"hotkey": "-e", "type": "switch"},
    "matchCase": {"hotkey": "-m", "type": "switch"},
    "all": {"hotkey": "-a", "type": "switch"},
    "custom": {"hotkey": "-c", "type": "switch"},
    "prefund": {"hotkey": "-p", "type": "switch"},
    "addr": {"hotkey": "-s", "type": "switch"},
    "tags": {"hotkey": "-g", "type": "switch"},
    "clean": {"hotkey": "-C", "type": "switch"},
    "regular": {"hotkey": "-r", "type": "switch"},
    "dryRun": {"hotkey": "-d", "type": "switch"},
    "autoname": {"hotkey": "-A", "type": "flag"},
    "create": {"hotkey": "", "type": "switch"},
    "update": {"hotkey": "", "type": "switch"},
    "delete": {"hotkey": "", "type": "switch"},
    "undelete": {"hotkey": "", "type": "switch"},
    "remove": {"hotkey": "", "type": "switch"},
    "chain": {"hotkey": "", "type": "flag"},
    "noHeader": {"hotkey": "", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
}

def names(self):
    ret = self.toUrl(namesCmd, namesPos, namesFmt, namesOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text

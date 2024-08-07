# Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
# Use of this source code is governed by a license that can
# be found in the LICENSE file.

"""
This file was auto generated. DO NOT EDIT.
"""

from . import session

slurpCmd = "slurp"
slurpPos = "addrs"
slurpFmt = "json"
slurpOpts = {
    "parts": {"hotkey": "-r", "type": "flag"},
    "appearances": {"hotkey": "-p", "type": "switch"},
    "articulate": {"hotkey": "-a", "type": "switch"},
    "source": {"hotkey": "-S", "type": "flag"},
    "count": {"hotkey": "-U", "type": "switch"},
    "page": {"hotkey": "-g", "type": "flag"},
    "pageId": {"hotkey": "", "type": "flag"},
    "perPage": {"hotkey": "-P", "type": "flag"},
    "sleep": {"hotkey": "-s", "type": "flag"},
    "chain": {"hotkey": "", "type": "flag"},
    "noHeader": {"hotkey": "", "type": "switch"},
    "cache": {"hotkey": "-o", "type": "switch"},
    "decache": {"hotkey": "-D", "type": "switch"},
    "ether": {"hotkey": "-H", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
}

def slurp(self):
    ret = self.toUrl(slurpCmd, slurpPos, slurpFmt, slurpOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text

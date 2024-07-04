# Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
# Use of this source code is governed by a license that can
# be found in the LICENSE file.

"""
This file was auto generated. DO NOT EDIT.
"""

from . import session

initCmd = "init"
initPos = ""
initFmt = "json"
initOpts = {
    "all": {"hotkey": "-a", "type": "switch"},
    "example": {"hotkey": "-e", "type": "flag"},
    "dryRun": {"hotkey": "-d", "type": "switch"},
    "firstBlock": {"hotkey": "-F", "type": "flag"},
    "sleep": {"hotkey": "-s", "type": "flag"},
    "chain": {"hotkey": "", "type": "flag"},
}

def init(self):
    ret = self.toUrl(initCmd, initPos, initFmt, initOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text

# Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
# Use of this source code is governed by a license that can
# be found in the LICENSE file.

"""
This file was auto generated. DO NOT EDIT.
"""

from . import session

configCmd = "config"
configPos = "mode"
configFmt = "json"
configOpts = {
    "paths": {"hotkey": "-a", "type": "switch"},
    "chain": {"hotkey": "", "type": "flag"},
    "noHeader": {"hotkey": "", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
}

def config(self):
    ret = self.toUrl(configCmd, configPos, configFmt, configOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text

#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

configCmd = "config"
configPos = "mode"
configFmt = "json"
configOpts = {
    "paths": {"hotkey": "-a", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
    "verbose:": {"hotkey": "-v", "type": "switch"},
    "help": {"hotkey": "-h", "type": "switch"},
}

def config(self):
    ret = self.toUrl(configCmd, configPos, configFmt, configOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text


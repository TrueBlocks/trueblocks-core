#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

abisCmd = "abis"
abisPos = "addrs"
abisFmt = "json"
abisOpts = {
    "known": {"hotkey": "-k", "type": "switch"},
    "find": {"hotkey": "-f", "type": "flag"},
    "hint": {"hotkey": "-n", "type": "flag"},
    "encode": {"hotkey": "-e", "type": "flag"},
    "clean": {"hotkey": "-C", "type": "flag"},
    "fmt": {"hotkey": "-x", "type": "flag"},
    "verbose:": {"hotkey": "-v", "type": "switch"},
    "help": {"hotkey": "-h", "type": "switch"},
}

def abis(self):
    ret = self.toUrl(abisCmd, abisPos, abisFmt, abisOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text


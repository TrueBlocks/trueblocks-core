#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

abisCmd = "abis"
abisPos = "addrs"
abisFmt = "json"
abisOpts = {
    "known": {"hotkey": "-k", "type": "switch"},
    "proxyFor": {"hotkey": "-r", "type": "flag"},
    "find": {"hotkey": "-f", "type": "flag"},
    "hint": {"hotkey": "-n", "type": "flag"},
    "encode": {"hotkey": "-e", "type": "flag"},
    "fmt": {"hotkey": "txt", "type": "flag"},
    "chain": {"hotkey": "-", "type": "flag"},
    "noHeader": {"hotkey": "-", "type": "switch"},
    "cache": {"hotkey": "-o", "type": "switch"},
    "decache": {"hotkey": "-D", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
}

def abis(self):
    ret = self.toUrl(abisCmd, abisPos, abisFmt, abisOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text

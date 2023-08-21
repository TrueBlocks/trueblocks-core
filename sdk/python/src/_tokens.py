#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

tokensCmd = "tokens"
tokensPos = "addrs"
tokensFmt = "json"
tokensOpts = {
    "parts": {"hotkey": "-p", "type": "flag"},
    "byAcct": {"hotkey": "-b", "type": "switch"},
    "changes": {"hotkey": "-c", "type": "switch"},
    "noZero": {"hotkey": "-z", "type": "switch"},
    "cache": {"hotkey": "-o", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
    "verbose:": {"hotkey": "-v", "type": "switch"},
    "help": {"hotkey": "-h", "type": "switch"},
}

def tokens(self):
    ret = self.toUrl(tokensCmd, tokensPos, tokensFmt, tokensOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text


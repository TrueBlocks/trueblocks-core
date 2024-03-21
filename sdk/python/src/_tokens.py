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
    "chain": {"hotkey": "-", "type": "flag"},
    "noHeader": {"hotkey": "-", "type": "switch"},
    "cache": {"hotkey": "-o", "type": "switch"},
    "decache": {"hotkey": "-D", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
}

def tokens(self):
    ret = self.toUrl(tokensCmd, tokensPos, tokensFmt, tokensOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text

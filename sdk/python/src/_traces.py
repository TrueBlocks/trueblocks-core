#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

tracesCmd = "traces"
tracesPos = "transactions"
tracesFmt = "json"
tracesOpts = {
    "articulate": {"hotkey": "-a", "type": "switch"},
    "filter": {"hotkey": "-f", "type": "flag"},
    "count": {"hotkey": "-U", "type": "switch"},
    "fmt": {"hotkey": "txt", "type": "flag"},
    "chain": {"hotkey": "-", "type": "flag"},
    "noHeader": {"hotkey": "-", "type": "switch"},
    "cache": {"hotkey": "-o", "type": "switch"},
    "decache": {"hotkey": "-D", "type": "switch"},
    "ether": {"hotkey": "-H", "type": "switch"},
    "raw": {"hotkey": "-w", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
}

def traces(self):
    ret = self.toUrl(tracesCmd, tracesPos, tracesFmt, tracesOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text


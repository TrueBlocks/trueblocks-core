#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

slurpCmd = "slurp"
slurpPos = "addrs"
slurpFmt = "json"
slurpOpts = {
    "types": {"hotkey": "-t", "type": "flag"},
    "appearances": {"hotkey": "-p", "type": "switch"},
    "articulate": {"hotkey": "-a", "type": "switch"},
    "source": {"hotkey": "-S", "type": "flag"},
    "count": {"hotkey": "-U", "type": "switch"},
    "sleep": {"hotkey": "-s", "type": "flag"},
    "fmt": {"hotkey": "txt", "type": "flag"},
    "chain": {"hotkey": "-", "type": "flag"},
    "noHeader": {"hotkey": "-", "type": "switch"},
    "cache": {"hotkey": "-o", "type": "switch"},
    "decache": {"hotkey": "-D", "type": "switch"},
    "ether": {"hotkey": "-H", "type": "switch"},
    "raw": {"hotkey": "-w", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
}

def slurp(self):
    ret = self.toUrl(slurpCmd, slurpPos, slurpFmt, slurpOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text


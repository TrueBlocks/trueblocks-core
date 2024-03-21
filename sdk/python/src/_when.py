#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

whenCmd = "when"
whenPos = "blocks"
whenFmt = "json"
whenOpts = {
    "list": {"hotkey": "-l", "type": "switch"},
    "timestamps": {"hotkey": "-t", "type": "switch"},
    "count": {"hotkey": "-U", "type": "switch"},
    "repair": {"hotkey": "-r", "type": "switch"},
    "check": {"hotkey": "-c", "type": "switch"},
    "update": {"hotkey": "-u", "type": "switch"},
    "deep": {"hotkey": "-d", "type": "switch"},
    "chain": {"hotkey": "-", "type": "flag"},
    "noHeader": {"hotkey": "-", "type": "switch"},
    "cache": {"hotkey": "-o", "type": "switch"},
    "decache": {"hotkey": "-D", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
}

def when(self):
    ret = self.toUrl(whenCmd, whenPos, whenFmt, whenOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text

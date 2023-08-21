#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

statusCmd = "status"
statusPos = "modes"
statusFmt = "json"
statusOpts = {
    "firstRecord": {"hotkey": "-c", "type": "flag"},
    "maxRecords": {"hotkey": "-e", "type": "flag"},
    "chains": {"hotkey": "-a", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
    "verbose:": {"hotkey": "-v", "type": "switch"},
    "help": {"hotkey": "-h", "type": "switch"},
}

def status(self):
    ret = self.toUrl(statusCmd, statusPos, statusFmt, statusOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text


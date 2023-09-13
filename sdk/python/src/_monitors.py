#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

monitorsCmd = "monitors"
monitorsPos = "addrs"
monitorsFmt = "json"
monitorsOpts = {
    "delete": {"hotkey": "", "type": "switch"},
    "undelete": {"hotkey": "", "type": "switch"},
    "remove": {"hotkey": "", "type": "switch"},
    "clean": {"hotkey": "-C", "type": "switch"},
    "list": {"hotkey": "-l", "type": "switch"},
    "watch": {"hotkey": "-w", "type": "switch"},
    "watchlist": {"hotkey": "-a", "type": "flag"},
    "commands": {"hotkey": "-c", "type": "flag"},
    "batchSize": {"hotkey": "-b", "type": "flag"},
    "sleep": {"hotkey": "-s", "type": "flag"},
    "fmt": {"hotkey": "-x", "type": "flag"},
    "verbose:": {"hotkey": "-v", "type": "switch"},
    "help": {"hotkey": "-h", "type": "switch"},
}

def monitors(self):
    ret = self.toUrl(monitorsCmd, monitorsPos, monitorsFmt, monitorsOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text


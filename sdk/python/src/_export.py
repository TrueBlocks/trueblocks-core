#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

exportCmd = "export"
exportPos = "addrs"
exportFmt = "json"
exportOpts = {
    "appearances": {"hotkey": "-p", "type": "switch"},
    "receipts": {"hotkey": "-r", "type": "switch"},
    "logs": {"hotkey": "-l", "type": "switch"},
    "traces": {"hotkey": "-t", "type": "switch"},
    "neighbors": {"hotkey": "-n", "type": "switch"},
    "accounting": {"hotkey": "-C", "type": "switch"},
    "statements": {"hotkey": "-A", "type": "switch"},
    "balances": {"hotkey": "-b", "type": "switch"},
    "articulate": {"hotkey": "-a", "type": "switch"},
    "cacheTraces": {"hotkey": "-R", "type": "switch"},
    "count": {"hotkey": "-U", "type": "switch"},
    "firstRecord": {"hotkey": "-c", "type": "flag"},
    "maxRecords": {"hotkey": "-e", "type": "flag"},
    "relevant": {"hotkey": "-N", "type": "switch"},
    "emitter": {"hotkey": "-m", "type": "flag"},
    "topic": {"hotkey": "-B", "type": "flag"},
    "asset": {"hotkey": "-P", "type": "flag"},
    "flow": {"hotkey": "-f", "type": "flag"},
    "factory": {"hotkey": "-y", "type": "switch"},
    "unripe": {"hotkey": "-u", "type": "switch"},
    "load": {"hotkey": "-O", "type": "flag"},
    "reversed": {"hotkey": "-E", "type": "switch"},
    "noZero": {"hotkey": "-z", "type": "switch"},
    "firstBlock": {"hotkey": "-F", "type": "flag"},
    "lastBlock": {"hotkey": "-L", "type": "flag"},
    "fmt": {"hotkey": "-x", "type": "flag"},
    "verbose:": {"hotkey": "-v", "type": "switch"},
    "help": {"hotkey": "-h", "type": "switch"},
}

def export(self):
    ret = self.toUrl(exportCmd, exportPos, exportFmt, exportOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text


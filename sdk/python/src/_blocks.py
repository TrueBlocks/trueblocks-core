#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

blocksCmd = "blocks"
blocksPos = "blocks"
blocksFmt = "json"
blocksOpts = {
    "hashes": {"hotkey": "-e", "type": "switch"},
    "uncles": {"hotkey": "-c", "type": "switch"},
    "traces": {"hotkey": "-t", "type": "switch"},
    "uniq": {"hotkey": "-u", "type": "switch"},
    "flow": {"hotkey": "-f", "type": "flag"},
    "logs": {"hotkey": "-l", "type": "switch"},
    "emitter": {"hotkey": "-m", "type": "flag"},
    "topic": {"hotkey": "-B", "type": "flag"},
    "articulate": {"hotkey": "-a", "type": "switch"},
    "bigRange": {"hotkey": "-r", "type": "flag"},
    "count": {"hotkey": "-U", "type": "switch"},
    "cacheTxs": {"hotkey": "", "type": "switch"},
    "cacheTraces": {"hotkey": "", "type": "switch"},
    "list": {"hotkey": "-L", "type": "flag"},
    "listCount": {"hotkey": "-C", "type": "flag"},
    "fmt": {"hotkey": "-x", "type": "flag"},
    "verbose:": {"hotkey": "-v", "type": "switch"},
    "help": {"hotkey": "-h", "type": "switch"},
}

def blocks(self):
    ret = self.toUrl(blocksCmd, blocksPos, blocksFmt, blocksOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text


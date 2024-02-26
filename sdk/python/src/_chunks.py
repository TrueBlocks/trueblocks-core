#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

chunksCmd = "chunks"
chunksPos = "mode"
chunksFmt = "json"
chunksOpts = {
    "check": {"hotkey": "-c", "type": "switch"},
    "pin": {"hotkey": "-i", "type": "switch"},
    "publish": {"hotkey": "-p", "type": "switch"},
    "remote": {"hotkey": "-r", "type": "switch"},
    "belongs": {"hotkey": "-b", "type": "flag"},
    "firstBlock": {"hotkey": "-F", "type": "flag"},
    "lastBlock": {"hotkey": "-L", "type": "flag"},
    "maxAddrs": {"hotkey": "-m", "type": "flag"},
    "deep": {"hotkey": "-d", "type": "switch"},
    "rewrite": {"hotkey": "-e", "type": "switch"},
    "count": {"hotkey": "-U", "type": "switch"},
    "sleep": {"hotkey": "-s", "type": "flag"},
    "fmt": {"hotkey": "txt", "type": "flag"},
    "chain": {"hotkey": "-", "type": "flag"},
    "noHeader": {"hotkey": "-", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
}

def chunks(self):
    ret = self.toUrl(chunksCmd, chunksPos, chunksFmt, chunksOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text


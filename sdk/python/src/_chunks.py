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
    "publisher": {"hotkey": "-P", "type": "flag"},
    "remote": {"hotkey": "-r", "type": "switch"},
    "belongs": {"hotkey": "-b", "type": "flag"},
    "firstBlock": {"hotkey": "-F", "type": "flag"},
    "lastBlock": {"hotkey": "-L", "type": "flag"},
    "maxAddrs": {"hotkey": "-m", "type": "flag"},
    "deep": {"hotkey": "-d", "type": "switch"},
    "sleep": {"hotkey": "-s", "type": "flag"},
    "fmt": {"hotkey": "-x", "type": "flag"},
    "verbose:": {"hotkey": "-v", "type": "switch"},
    "help": {"hotkey": "-h", "type": "switch"},
}

def chunks(self):
    ret = self.toUrl(chunksCmd, chunksPos, chunksFmt, chunksOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text


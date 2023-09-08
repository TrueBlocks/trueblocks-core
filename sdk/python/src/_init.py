#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

initCmd = "init"
initPos = ""
initFmt = "json"
initOpts = {
    "all": {"hotkey": "-a", "type": "switch"},
    "dryRun": {"hotkey": "-d", "type": "switch"},
    "publisher": {"hotkey": "-P", "type": "flag"},
    "firstBlock": {"hotkey": "-F", "type": "flag"},
    "sleep": {"hotkey": "-s", "type": "flag"},
    "fmt": {"hotkey": "-x", "type": "flag"},
    "verbose:": {"hotkey": "-v", "type": "switch"},
    "help": {"hotkey": "-h", "type": "switch"},
}

def init(self):
    ret = self.toUrl(initCmd, initPos, initFmt, initOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text


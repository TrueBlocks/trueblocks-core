#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

logsCmd = "logs"
logsPos = "transactions"
logsFmt = "json"
logsOpts = {
    "articulate": {"hotkey": "-a", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
    "verbose:": {"hotkey": "-v", "type": "switch"},
    "help": {"hotkey": "-h", "type": "switch"},
}

def logs(self):
    ret = self.toUrl(logsCmd, logsPos, logsFmt, logsOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text


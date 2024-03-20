#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

transactionsCmd = "transactions"
transactionsPos = "transactions"
transactionsFmt = "json"
transactionsOpts = {
    "articulate": {"hotkey": "-a", "type": "switch"},
    "traces": {"hotkey": "-t", "type": "switch"},
    "uniq": {"hotkey": "-u", "type": "switch"},
    "flow": {"hotkey": "-f", "type": "flag"},
    "logs": {"hotkey": "-l", "type": "switch"},
    "emitter": {"hotkey": "-m", "type": "flag"},
    "topic": {"hotkey": "-B", "type": "flag"},
    "chain": {"hotkey": "-", "type": "flag"},
    "noHeader": {"hotkey": "-", "type": "switch"},
    "cache": {"hotkey": "-o", "type": "switch"},
    "decache": {"hotkey": "-D", "type": "switch"},
    "ether": {"hotkey": "-H", "type": "switch"},
    "raw": {"hotkey": "-w", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
}

def transactions(self):
    ret = self.toUrl(transactionsCmd, transactionsPos, transactionsFmt, transactionsOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text

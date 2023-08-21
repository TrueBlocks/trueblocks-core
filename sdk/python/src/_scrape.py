#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

scrapeCmd = "scrape"
scrapePos = ""
scrapeFmt = "json"
scrapeOpts = {
    "blockCnt": {"hotkey": "-n", "type": "flag"},
    "pin": {"hotkey": "-i", "type": "switch"},
    "remote": {"hotkey": "-r", "type": "switch"},
    "sleep": {"hotkey": "-s", "type": "flag"},
    "startBlock": {"hotkey": "-l", "type": "flag"},
    "fmt": {"hotkey": "-x", "type": "flag"},
    "verbose:": {"hotkey": "-v", "type": "switch"},
    "help": {"hotkey": "-h", "type": "switch"},
}

def scrape(self):
    ret = self.toUrl(scrapeCmd, scrapePos, scrapeFmt, scrapeOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text


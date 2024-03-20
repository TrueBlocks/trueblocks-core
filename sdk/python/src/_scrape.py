#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

scrapeCmd = "scrape"
scrapePos = ""
scrapeFmt = "json"
scrapeOpts = {
    "blockCnt": {"hotkey": "-n", "type": "flag"},
    "sleep": {"hotkey": "-s", "type": "flag"},
    "touch": {"hotkey": "-l", "type": "flag"},
    "chain": {"hotkey": "-", "type": "flag"},
    "fmt": {"hotkey": "-x", "type": "flag"},
}

def scrape(self):
    ret = self.toUrl(scrapeCmd, scrapePos, scrapeFmt, scrapeOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text

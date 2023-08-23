import sys, os

from . import session

def isValidCommand(self):
    if len(sys.argv) == 0:
        return False
    
    cmdMap = {
        "abis": True,
        "blocks": True,
        "chunks": True,
        "config": True,
        "daemon": True,
        "explore": True,
        "export": True,
        "init": True,
        "list": True,
        "logs": True,
        "monitors": True,
        "names": True,
        "receipts": True,
        "scrape": True,
        "slurp": True,
        "state": True,
        "tokens": True,
        "traces": True,
        "transactions": True,
        "when": True,
    }
    cmd = sys.argv[1]
    return cmd in cmdMap

def usage(self, msg):
    print(msg)
    os.system("chifra --help")
    exit(1)
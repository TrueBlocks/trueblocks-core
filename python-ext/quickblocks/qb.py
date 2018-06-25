import _quickblocks as qbe
import json

class QuickBlocks(object):
    def __init__(self, url, cache):
        qbe.init(url)
        self.cache = True

    def __getitem__(self, blockNum):
        return json.loads(qbe.get_block(blockNum, self.cache))

    def __del__(self):
        qbe.cleanup()

"""
qblocks - fast, easily-accessible, fully-decentralized data from blockchains
copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
This program is free software: you may redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version. This program is
distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details. You should have received a copy of the GNU General
Public License along with this program. If not, see http://www.gnu.org/licenses/.
"""

import _quickblocks as qbe
import json
from quickblocks.Timer import my_timer
class QuickBlocks(object):
    def __init__(self, url, cache):
        qbe.init(url)
        self.cache = True

    def __getitem__(self, blockNum):
        return json.loads(qbe.get_block(blockNum, self.cache))

    def __del__(self):
        qbe.cleanup()

    @my_timer   
    def getTransaction(self, **kwargs):
        if(kwargs.get('txhash')):
            result=qbe.get_trans(kwargs.get('txhash'))
        
        
        return result  


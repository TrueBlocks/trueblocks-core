#!/usr/bin/env python

import sys
import pprint
from src import chifra

obj = chifra().dispatch()
pprint.pprint(obj)

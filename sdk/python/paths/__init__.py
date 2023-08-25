# do not import all endpoints into this module because that uses a lot of memory and stack frames
# if you need the ability to import all endpoints from this module, import them with
# from openapi_client.apis.path_to_api import path_to_api

import enum


class PathValues(str, enum.Enum):
    LIST = "/list"
    EXPORT = "/export"
    MONITORS = "/monitors"
    NAMES = "/names"
    ABIS = "/abis"
    BLOCKS = "/blocks"
    TRANSACTIONS = "/transactions"
    RECEIPTS = "/receipts"
    LOGS = "/logs"
    TRACES = "/traces"
    WHEN = "/when"
    STATE = "/state"
    TOKENS = "/tokens"
    CONFIG = "/config"
    STATUS = "/status"
    SCRAPE = "/scrape"
    CHUNKS = "/chunks"
    INIT = "/init"
    SLURP = "/slurp"

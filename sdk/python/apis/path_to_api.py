import typing_extensions

from openapi_client.paths import PathValues
from openapi_client.apis.paths._list import ModelList
from openapi_client.apis.paths.export import Export
from openapi_client.apis.paths.monitors import Monitors
from openapi_client.apis.paths.names import Names
from openapi_client.apis.paths.abis import Abis
from openapi_client.apis.paths.blocks import Blocks
from openapi_client.apis.paths.transactions import Transactions
from openapi_client.apis.paths.receipts import Receipts
from openapi_client.apis.paths.logs import Logs
from openapi_client.apis.paths.traces import Traces
from openapi_client.apis.paths.when import When
from openapi_client.apis.paths.state import State
from openapi_client.apis.paths.tokens import Tokens
from openapi_client.apis.paths.config import Config
from openapi_client.apis.paths.status import Status
from openapi_client.apis.paths.scrape import Scrape
from openapi_client.apis.paths.chunks import Chunks
from openapi_client.apis.paths.init import Init
from openapi_client.apis.paths.slurp import Slurp

PathToApi = typing_extensions.TypedDict(
    'PathToApi',
    {
        PathValues.LIST: ModelList,
        PathValues.EXPORT: Export,
        PathValues.MONITORS: Monitors,
        PathValues.NAMES: Names,
        PathValues.ABIS: Abis,
        PathValues.BLOCKS: Blocks,
        PathValues.TRANSACTIONS: Transactions,
        PathValues.RECEIPTS: Receipts,
        PathValues.LOGS: Logs,
        PathValues.TRACES: Traces,
        PathValues.WHEN: When,
        PathValues.STATE: State,
        PathValues.TOKENS: Tokens,
        PathValues.CONFIG: Config,
        PathValues.STATUS: Status,
        PathValues.SCRAPE: Scrape,
        PathValues.CHUNKS: Chunks,
        PathValues.INIT: Init,
        PathValues.SLURP: Slurp,
    }
)

path_to_api = PathToApi(
    {
        PathValues.LIST: ModelList,
        PathValues.EXPORT: Export,
        PathValues.MONITORS: Monitors,
        PathValues.NAMES: Names,
        PathValues.ABIS: Abis,
        PathValues.BLOCKS: Blocks,
        PathValues.TRANSACTIONS: Transactions,
        PathValues.RECEIPTS: Receipts,
        PathValues.LOGS: Logs,
        PathValues.TRACES: Traces,
        PathValues.WHEN: When,
        PathValues.STATE: State,
        PathValues.TOKENS: Tokens,
        PathValues.CONFIG: Config,
        PathValues.STATUS: Status,
        PathValues.SCRAPE: Scrape,
        PathValues.CHUNKS: Chunks,
        PathValues.INIT: Init,
        PathValues.SLURP: Slurp,
    }
)

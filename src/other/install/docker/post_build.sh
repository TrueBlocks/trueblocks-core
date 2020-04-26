touch /root/.quickBlocks/names/names_prefunds.txt
echo "apiProvider=\"http://localhost:80\"" >>/root/.quickBlocks/quickBlocks.toml
mkdir -pv /root/.quickBlocks/cache/addr_index/finalized
mkdir -pv /root/.quickBlocks/cache/addr_index/blooms
mkdir -pv /root/.quickBlocks/cache/addr_index/staging
mkdir -pv /root/.quickBlocks/cache/addr_index/ripe
mkdir -pv /root/.quickBlocks/cache/addr_index/unripe
mkdir -pv /root/.quickBlocks/cache/addr_index/finalized
mkdir -pv /root/.quickBlocks/cache/monitors
mkdir -pv /root/.quickBlocks/cache/prices
mkdir -pv /root/.quickBlocks/cache/tmp
mkdir -pv /root/.quickBlocks/cache/blocks
mkdir -pv /root/.quickBlocks/cache/txs
mkdir -pv /root/.quickBlocks/cache/traces
mkdir -pv /root/.quickBlocks/cache/slurps
cat "/root/.quickBlocks/quickBlocks.toml"

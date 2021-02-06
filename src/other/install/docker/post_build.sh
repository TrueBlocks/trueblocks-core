touch /root/.quickBlocks/names/names_prefunds.txt
echo "apiProvider=\"http://localhost:80\"" >>/root/.quickBlocks/quickBlocks.toml

mkdir /root/.quickBlocks/cache/
mkdir /root/.quickBlocks/cache/abis
mkdir /root/.quickBlocks/cache/addr_index
mkdir /root/.quickBlocks/cache/addr_index/blooms
mkdir /root/.quickBlocks/cache/addr_index/finalized
mkdir /root/.quickBlocks/cache/addr_index/ripe
mkdir /root/.quickBlocks/cache/addr_index/staging
mkdir /root/.quickBlocks/cache/addr_index/unripe
mkdir /root/.quickBlocks/cache/monitors
mkdir /root/.quickBlocks/cache/monitors/staging
mkdir /root/.quickBlocks/cache/prices
mkdir /root/.quickBlocks/cache/tmp

mkdir /root/.quickBlocks/abis
mkdir /root/.quickBlocks/chifra
mkdir /root/.quickBlocks/grabABI
mkdir /root/.quickBlocks/grabABI/docs
mkdir /root/.quickBlocks/grabABI/parselib
mkdir /root/.quickBlocks/ipfs-hashes
mkdir /root/.quickBlocks/ipfs-hashes/data
mkdir /root/.quickBlocks/makeClass
mkdir /root/.quickBlocks/names

#cat "/root/.quickBlocks/quickBlocks.toml"

ls -lR "/root/.quickBlocks/"

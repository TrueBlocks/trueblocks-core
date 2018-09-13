//! Warning! Uses Etherscan
mod cache;
mod simpledb;
pub mod err;

use log::*;
use serde_derive::Deserialize;
use failure::{Error, ResultExt};
use rayon::prelude::*;
use futures::{
    future::Future,
    stream::Stream,
    Poll,
    sync::mpsc::{self, UnboundedSender, UnboundedReceiver},
};
use web3::{
    BatchTransport,
    types::{Address, Trace, Transaction, TransactionId, BlockNumber, TransactionReceipt, H160, BlockId, Block as Web3Block, H256, Index},
};
use std::path::PathBuf;
use super::{
    utils,
    etherscan::{EtherScan, SortType},
    client::{Client},
    err::{ErrorKind, ValidateMsg},
};

use self::cache::{TxType, Block, TransactionCache as Cache};

#[derive(Deserialize, Debug, Clone, PartialEq)]
pub struct TxEntry  {
    #[serde(rename = "blockNum")]
    block_num: u64,
    #[serde(rename = "transactionIndex")]
    transaction_index: usize,
    location: String,
}

impl std::fmt::Display for TxEntry {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(f, "block: {}, index: {}, location: {}", self.block_num, self.transaction_index, self.location)
    }
}

#[derive(Debug, Clone)]
pub enum InvalidEntry {
    Missing(H256),
    Incorrect(TxEntry, Option<H256>) // hash is None if transction does not exist on ethereum mainnet
}

impl std::fmt::Display for InvalidEntry {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            InvalidEntry::Incorrect(entry, hash) => {
                if hash.is_some() {
                    write!(f, "Incorrect Entry at {} with hash: {:x}", entry, hash.expect("scope is conditional; qed"))
                } else {
                    write!(f, "Incorrect Entry at {}, Transaction does not exist", entry)
                }
            }
            InvalidEntry::Missing(hash) => write!(f, "Missing Transaction {:x}", hash),
        }
    }
}

pub struct TransactionValidator {
    csv: Vec<TxEntry>,
    cache: Cache,
    addr: Address,
}

//TODO: skip DOS transactions (blocks 2283440 -- 2718436 with > 250 traces) #p1
//https://medium.com/@tjayrush/defeating-the-ethereum-ddos-attacks-d3d773a9a063
//
impl TransactionValidator  {
    //creates a new validator from genesis to specified block
    pub fn new<T>(client: &mut Client<T>, csv_file: PathBuf, to_block: Option<BlockNumber>, address: H160)
                  -> Result<Self, Error>
    where
        T: BatchTransport + Send + Sync + 'static
    {
        let mut csv_vec = Vec::new();
        let mut rdr = csv::Reader::from_path(csv_file.as_path())
            .with_context(|e| format!("Could not get csv file {}", e))?;
        for result in rdr.deserialize() {
            let res: TxEntry = result.context(ErrorKind::Validate(ValidateMsg::InvalidCsv))?;
            csv_vec.push(res);
        }

        // discard write handle; we should never modify the original CSV
        let to_block = to_block.unwrap_or(BlockNumber::Latest);
        Ok(TransactionValidator {
            csv: csv_vec,
            cache: Self::build_local_cache(client, to_block, address)?,
            addr: address,
        })
    }

    fn build_local_cache<T>(client: &mut Client<T>, to_block: BlockNumber, addr: H160) -> Result<Cache, Error>
    where
        T: BatchTransport + Send + Sync + 'static,
    {
        let mut cache = Cache::new(addr, BlockNumber::Earliest, to_block)?;
        if cache.is_populated() {
            return Ok(cache);
        }

        info!("gathering transactions from EtherScan");
        let eth_scan = EtherScan::new();
        let to_block = utils::as_u64(client, to_block);
        let hashes = eth_scan.get_tx_by_account(client.ev_loop(), addr, 0, to_block, SortType::Ascending)?;

        // gather these in four asynchronous calls. This works best if the node being used
        // allows for 3+ threads for RPC calls
        let (txs, receipts, traces, blocks) = (client.batch(), client.batch(), client.batch(), client.batch());

        let mut block_numbers: Vec<(H256, u64)> = Vec::new();
        for (hash, block_num) in hashes.iter() {
            txs.eth().transaction(TransactionId::Hash(*hash));
            receipts.eth().transaction_receipt(*hash);
            traces.trace().transaction(*hash);
            // only get block number if we haven't gotten it yet
            // associates a TXHash with a blocknumber. important later when we create Block{} struct
            // make seperate storage for blocks
            // deserialize into Bincode/Message pack and compress
            match block_numbers.binary_search_by_key(block_num, |&(_, blk_num)| blk_num) {
                Ok(_) => {}, //ele already exists
                Err(pos) => {
                    block_numbers.insert(pos, (*hash, *block_num));
                    blocks.eth().block(BlockId::Number(BlockNumber::Number(*block_num)));
                }
            }
        }

        let (sender, receiver) = mpsc::unbounded();
        let txs = cache_get_task::<Transaction, _, _>(txs.transport(), sender.clone(), |val| {TxType::from(val)}); // .then here
        let receipts = cache_get_task::<TransactionReceipt, _, _>(receipts.transport(), sender.clone(), |val|{TxType::from(val)});
        let traces = cache_get_task::<Vec<Trace>, _, _>(traces.transport(), sender.clone(), |val| {TxType::from(val)});

        let blocks = cache_get_task::<Web3Block<H256>, _, _>(blocks.transport(), sender, move |val| {
            let pos = &block_numbers
                .binary_search_by_key(&val.number.expect(&verb_msg!("block num should not be pending")).as_u64(), |(_, blk_num)| *blk_num).unwrap();
            let (tx, _) = block_numbers.get(*pos).unwrap().clone();
            TxType::from(Block { tx_hash: tx, block: val })
        });

        client.handle().spawn(txs);
        client.handle().spawn(receipts);
        client.handle().spawn(traces);
        client.handle().spawn(blocks);

        let fut = receiver.for_each(|tx_type| {
            cache.insert(tx_type);
            Ok(())
        });

        info!("Submitting batch requests of Transactions, Receipts, and Traces");
        client.run(fut).unwrap();
        info!("Finished building local cache. Saving...");
        cache.save()?;
        Ok(cache)
    }

    // attempts to validate csv list of transactions, returning any incorrectly included
    // transactions
    // return a stream of missing, or incorrectly included transactions
    /// 'dedup' CSV list to include only transactions with unique block number and transaction index
    pub fn scan<T>(&self, client: &Client<T>) -> Result<Scan, Error>
    where
        T: BatchTransport + Send + Sync + 'static,
        <T as web3::Transport>::Out: Send
    {
        let (tx, rx): (UnboundedSender<InvalidEntry>, UnboundedReceiver<InvalidEntry>) = mpsc::unbounded();
        self.find_misplaced(client, tx.clone())?;
        Ok(Scan { inner: rx })
    }

    /// find transactions that were incorrectly included in the CSV
    fn find_misplaced<T>(&self, client: &Client<T>, sender: UnboundedSender<InvalidEntry>) -> Result<(), Error>
    where
        T: BatchTransport + Send + Sync + 'static,
        <T as web3::Transport>::Out: Send,
    {
        let mut csv = self.csv.clone();
        let mut new_csv = Vec::new();
        let mut added:  Vec<(u64, usize)> = Vec::new();
        for entry in csv {
            match added.binary_search(&(entry.block_num, entry.transaction_index)) {
                Ok(_) => {}, //added
                Err(pos) => {
                    added.insert(pos, (entry.block_num, entry.transaction_index));
                    new_csv.push(entry);
                }
            }
        }
        csv = new_csv;

        let remote = client.remote();
        let eth = client.web3.eth();
        let addr = self.addr.clone();
        csv.par_iter().for_each(|entry| {
            let entry = entry.clone();
            // check if the transaction is included in our cache
            if let None = self.cache.tx_by_blocknum_index(entry.block_num, entry.transaction_index) {
                let sender_async = sender.clone();
                // our database did not find the transaction -- but that does not yet mean the transaction is incorrectly included in quickblocks.
                // etherscan does not report transactions with the address included as 'data'.
                // query our node for the transaction to check if it is in the input
                let fut = eth
                    .transaction(TransactionId::Block(BlockId::Number(BlockNumber::Number(entry.block_num as u64)), Index::from(entry.transaction_index)))
                    .then(move |t| {
                        let t = try_web3!(t);
                        if let Some(tx) = t {
                            if !scan_bytes(addr, &tx.input.0) /* addr doesn't exist in input */ {
                                sender_async.unbounded_send(InvalidEntry::Incorrect(entry, Some(tx.hash))).unwrap();
                                drop(sender_async);
                            }
                        }
                        Ok(())
                    });
                remote.spawn(|_| {fut});
            }
        });
        Ok(())
    }
}

fn scan_bytes(addr: H160, bytes: &Vec<u8>) -> bool {
    bytes.windows(addr.len()).position(|window| &(*addr) == window).is_some()
}


/// asynchronously send a batch request
// decides on conversion through the predicate F
fn cache_get_task<A, T, F>(batch: &web3::transports::Batch<T>, sender: UnboundedSender<TxType>, fun: F) -> impl Future<Item=(), Error=()>
where
    A: serde::de::DeserializeOwned,
    T: BatchTransport,
    F: Fn(A) -> TxType,
{
    batch.submit_batch()
        .map_err(|e| ErrorKind::Network(format!("{}", e)).into())
        .and_then(|vals| {
            let res = vals.into_iter()
                .map(|val| {
                    serde_json::from_value(try_web3!(val)).map_err(|e| e.into())
                })
                .collect::<Result<Vec<A>, Error>>();
            futures::future::result(res)
        }).and_then(move |vals| {
            match vals
                .into_iter()
                .try_for_each(|val| sender.unbounded_send(fun(val))) {
                    Err(e) => error!("{}", verb_msg!("{}", e)),
                    Ok(_) => (),
                };
            drop(sender);
            Ok(())
        }).map_err(|e| {
            error!("{}", verb_msg!("{}", e));
            error!("{:?}", e);
            panic!("Shutting down");
        })
}

pub struct Scan {
    inner: UnboundedReceiver<InvalidEntry>
}

impl Stream for Scan {
    type Item = InvalidEntry;
    type Error = ();

    fn poll(self: &mut Self) -> Poll<Option<Self::Item>, Self::Error> {
        self.inner.poll()
    }
}


#[cfg(test)]
mod tests {
    use super::*;
    use web3::types::Address;
    use crate::conf::Configuration;
    use test::Bencher;

    fn tx_validator(client: &mut Client<web3::transports::http::Http>) -> TransactionValidator {
        match TransactionValidator::new(client,
                                       PathBuf::from("/home/insi/Projects/absentis/tx_list.csv"),
                                       Some(BlockNumber::Number(1_000_000)),
                                       Address::from("0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359"))
        {
            Err(e) => {
                let error = failure::Error::from(e);
                error!("{}", error.as_fail());
                error!("{:?}", error);
                panic!("failed due to ERROR");
            },
            Ok(v) => {
                info!("Len: {}", v.csv.len());
                info!("Success!");
                v
            }
        }
    }

    #[test]
    fn it_should_create_new_validator() {
        pretty_env_logger::try_init();
        let conf = Configuration::new().expect("Could not create configuration");
        let mut client = Client::<web3::transports::http::Http>::new_http(&conf).expect("Could not build client");
        let _validator = tx_validator(&mut client);
    }

    #[test]
    fn it_should_scan() {
        pretty_env_logger::try_init();
        let conf = Configuration::new().expect("Could not create configuration");
        let mut client = Client::<web3::transports::http::Http>::new_http(&conf).expect("Could not build client");
        let validator = tx_validator(&mut client);
        let fut = validator.scan(&client).unwrap().for_each(|inv| {
            info!("Invalid Transaction: {}", inv);
            Ok(())
        });
        client.run(fut).unwrap();
    }

    #[bench]
    fn bench_scan(b: &mut Bencher) {
        b.iter(|| {
            let conf = Configuration::new().expect("Could not create configuration");
            let mut client = Client::<web3::transports::http::Http>::new_http(&conf).expect("Could not build client");
            let validator = tx_validator(&mut client);
            let fut = validator.scan(&client).unwrap().for_each(|inv| {
                Ok(())
            });
            client.run(fut).unwrap();
        }) 
    }
}

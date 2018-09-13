//! A transaction cache for transaction_validator
use log::*;
use serde_derive::*;
use failure::{Error, ResultExt};
use std::{
    collections::HashMap,
    path::PathBuf,
};
use rayon::prelude::*;
use web3::types::{Transaction, TransactionReceipt, Trace, Log, H160, H256, U256, BlockNumber, Block as Web3Block, U128};
use crate::err::ErrorKind;

use super::{
    // intermediary_types::{self as db_types,TxInt, LogInt},
    simpledb::SimpleDB,
};

/// a simple cache for storing transactions
#[derive(Debug)]
pub struct TransactionCache {
    /// the cache
    cache: HashMap<H256, Tx>,
    /// name of an object in a database in OS temporary directory.
    name: String, // -- name convention = ADDRESS_FROMBLOCK_TOBLOCK
    db: SimpleDB<HashMap<H256, Tx>>,
    populated: bool,
}

/// A transaction and all associated information (Transaction, Receipt, Traces, Extra Logs)
#[derive(Debug, Clone, PartialEq, Deserialize, Serialize)]
pub struct Tx {
    pub transaction: Option<Transaction>,
    pub receipt: Option<TransactionReceipt>,
    pub traces: Option<Vec<Trace>>,
    pub logs: Option<Log>,
    pub block: Option<Block>
}

#[derive(Debug, Clone, PartialEq, Deserialize, Serialize)]
pub enum TxType {
    Transaction(Transaction),
    Receipt(TransactionReceipt),
    Traces(Vec<Trace>),
    Logs(Log),
    Block(Block)
}

#[derive(Debug, Clone, PartialEq, Deserialize, Serialize)]
pub struct Block {
    pub tx_hash: H256,
    pub block: Web3Block<H256>
}

impl std::fmt::Display for TxType {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            TxType::Transaction(_) => write!(f, "Transaction"),
            TxType::Receipt(_) => write!(f, "Receipt"),
            TxType::Traces(_) => write!(f, "Traces"),
            TxType::Logs(_) => write!(f, "Logs"),
            TxType::Block(_) => write!(f, "Block"),
        }
    }
}

trait Displayable {
    fn display(&self) -> String;
}
impl Displayable for BlockNumber {
    fn display(&self) -> String {
        match self {
            BlockNumber::Earliest => "earliest".to_string(),
            BlockNumber::Latest => "latest".to_string(),
            BlockNumber::Number(num) => num.to_string(),
            BlockNumber::Pending => "pending".to_string(),
        }
    }
}

// TODO: Make this a generic cache for H256 hashes #p3
// TODO: support drawing from multiple db files
//     - ie if an address is included multiple times like:
//     - {ADDR}_0_1000000_2000000 , {ADDR}_2000000_3000000
//     - and the user asks for transactions from 0 - 3000000, use both files instead of creating a new one
//     - or include a function to 'merge' consecutive files into {ADDR}_0_3000000
// doesn't do miner
impl TransactionCache {
    /// create a new Cache
    pub fn new(addr: H160, from_block: BlockNumber, to_block: BlockNumber) -> Result<Self, Error> {
        let name = format!("0x{:x}_{}_{}.bin", addr, from_block.display(), to_block.display());
        info!("FILE: {:?}", Self::try_local(&name)?);
        let db = Self::try_local(&name)?;
        let cache = db.get().context(ErrorKind::Cache)?;
        info!("cache.len(): {}", cache.len());
        Ok(TransactionCache {
            populated: cache.len() > 0,
            cache, name, db,
        })
    }

    /// Insert a TxType into Cache
    /// handle errors with .exists() to make sure we're not overwriting anything
    /// it is the job of the caller of insert() to make sure that no objects passed to insert have already been inserted for a tx hash
    /// Normally, this means that no TXhashes should be duplicates when getting information from the node
    pub fn insert(&mut self, tx: impl CacheAction) {
        if tx.exists(&self.cache) {
            error!("Transaction already exists in cache. Aborting...");
            std::process::exit(1);
        } else {
            tx.insert(&mut self.cache);
        }
    }

    /// extend cache with a vector of CacheAction Types
    crate fn extend(&mut self, val: Vec<impl CacheAction>) {
        self.cache.extend(val.into_iter().map(|x| (x.hash().clone(), x.empty())))
    }

    /// get a transaction hash from cache by block number
    crate fn txhash_by_blocknum_index(&self, block_num: u64, index: usize) -> Option<H256> {
        let block_num = U256::from(block_num);
        let index = U128::from(index);
        self.cache.par_iter()
            .find_any(|(_, v)| v.transaction.as_ref().unwrap().block_number.expect("Block number will never be pending; qed") == block_num && v.transaction.as_ref().unwrap().transaction_index.unwrap() == index)
            .map(|(k, _)| k.clone())
    }

    crate fn get(&self, tx_hash: &H256) -> Option<&Tx> {
        self.cache.get(tx_hash)
    }

    crate fn tx_by_blocknum_index(&self, block_num: u64, index: usize) -> Option<&Tx> {
        let tx_hash = self.txhash_by_blocknum_index(block_num, index)?;
        self.get(&tx_hash)
    }

    // clones cache
    /// Save all transactions to a temporary database that lives in system cache directory by cloning
    crate fn save(&mut self) -> Result<(), Error> {
        Ok(self.db.save(self.cache.clone()).context(ErrorKind::Cache)?)
    }

    /// check if cache contains transactions
    /// so far, this simply checks if the length of the underlying hashmap is > 0
    crate fn is_populated(&self) -> bool {
        self.populated
    }

    fn try_local(name: &str) -> Result<SimpleDB<HashMap<H256, Tx>>, Error> {
        if Self::db_exists(name)? {
            Ok(SimpleDB::<HashMap<H256, Tx>>::new(Self::db_path(name)?)?)
        } else {
            if !Self::dir_exists()? {
                std::fs::create_dir(Self::dir_path()?)?;
            }
            Ok(SimpleDB::<HashMap<H256, Tx>>::new(Self::db_path(name)?)?)
        }
    }

    fn db_path(name: &str) -> Result<PathBuf, Error> {
        let mut dir = Self::dir_path().context(ErrorKind::Cache)?;
        dir.push(name);
        Ok(dir)
    }

    fn db_exists(name: &str) -> Result<bool, Error> {
        let path = Self::db_path(name).context(ErrorKind::Cache)?;
        Ok(path.as_path().exists())
    }

    fn dir_path() -> Result<PathBuf, Error> {
        Ok(dirs::cache_dir().and_then(|mut d | {
            d.push("absentis");
            Some(d)
        }).ok_or(ErrorKind::Cache)?)
    }

    fn dir_exists() -> Result<bool, Error> {
        Ok(Self::dir_path()?.is_dir())
    }
}

// TODO: make this a macro #p2
impl CacheAction for TxType {
    fn insert(self, cache: &mut HashMap<H256, Tx>) {
        match self {
            TxType::Transaction(tx) => tx.insert(cache),
            TxType::Receipt(rec) => rec.insert(cache),
            TxType::Traces(tr) => tr.insert(cache),
            TxType::Logs(logs) => logs.insert(cache),
            TxType::Block(blk) => blk.insert(cache),
        }
    }

    fn exists(&self, cache: &HashMap<H256, Tx>) -> bool {
        match self {
            TxType::Transaction(tx) => tx.exists(cache),
            TxType::Receipt(rec) => rec.exists(cache),
            TxType::Traces(tr) => tr.exists(cache),
            TxType::Logs(logs) => logs.exists(cache),
            TxType::Block(blk) => blk.exists(cache),
        }
    }

    fn hash(&self) -> &H256 {
        match self {
            TxType::Transaction(tx) => tx.hash(),
            TxType::Receipt(rec) => rec.hash(),
            TxType::Traces(tr) => tr.hash(),
            TxType::Logs(logs) => logs.hash(),
            TxType::Block(blk) => blk.hash(),
        }
    }

    fn empty(self) -> Tx {
        match self {
            TxType::Transaction(tx) => tx.empty(),
            TxType::Receipt(rec) => rec.empty(),
            TxType::Traces(tr) => tr.empty(),
            TxType::Logs(logs) => logs.empty(),
            TxType::Block(blk) => blk.empty()
        }
    }
}

/// Common actions for web3 types in Cache
pub trait CacheAction {
    /// gets transaction hash of item in cache
    fn hash(&self) -> &H256;
    /// inserts an item into cache
    fn insert(self, cache: &mut HashMap<H256, Tx>);
    /// checks cache if this type exists within it
    fn exists(&self, cache: &HashMap<H256, Tx>) -> bool;
    /// converts CacheAction type to Tx type with all other fields of Tx as `None`
    fn empty(self) -> Tx;
}

impl CacheAction for Block {
    fn hash(&self) -> &H256 {
        &self.tx_hash
    }
    fn insert(self, cache: &mut HashMap<H256, Tx>) {
        if cache.contains_key(self.hash()) {
            let entry = cache.get_mut(self.hash()).expect("scope is conditional; qed");
            entry.block = Some(self);
        } else {
            cache.insert(self.hash().clone(), self.empty());
        }
    }
    fn exists(&self, cache: &HashMap<H256, Tx>) -> bool {
        cache.contains_key(self.hash()) && cache.get(self.hash()).expect("scope is conditional; qed").block.is_some()
    }

    fn empty(self) -> Tx {
        Tx { block: Some(self), transaction: None, logs: None, traces: None, receipt: None }
    }
}

impl CacheAction for Transaction {
    fn hash(&self) -> &H256 {
        &self.hash
    }
    fn insert(self, cache: &mut HashMap<H256, Tx>) {
        if cache.contains_key(self.hash()) {
            let entry = cache.get_mut(self.hash()).expect("scope is conditional; qed");
            entry.transaction = Some(self);
        } else {
            cache.insert(self.hash().clone(), self.empty());
        }
    }

    fn exists(&self, cache: &HashMap<H256, Tx>) -> bool {
        cache.contains_key(self.hash()) && cache.get(self.hash()).expect("scope is conditional; qed").transaction.is_some()
    }

    fn empty(self) -> Tx {
        Tx { transaction: Some(self), logs: None, traces: None, receipt: None, block: None }
    }
}

impl CacheAction for TransactionReceipt {
    fn hash(&self) -> &H256 {
        &self.transaction_hash
    }

    fn insert(self, cache: &mut HashMap<H256, Tx>) {
        if cache.contains_key(&self.transaction_hash) {
            let entry = cache.get_mut(&self.transaction_hash).expect("scope is conditional; qed");
            entry.receipt = Some(self);
        } else {
            cache.insert(self.hash().clone(), self.empty());
        }
    }

    fn exists(&self, cache: &HashMap<H256, Tx>) -> bool {
        cache.contains_key(self.hash()) && cache.get(self.hash()).expect("scope is conditional").receipt.is_some()
    }

    fn empty(self) -> Tx {
        Tx {receipt: Some(self), logs: None, traces: None, transaction: None, block: None}
    }
}

impl CacheAction for Vec<Trace> {
    fn hash(&self) -> &H256 {
        self.get(0).as_ref()
            .expect(&verb_msg!("Cannot insert an empty vector!"))
            .transaction_hash.as_ref()
            .expect(&verb_msg!("TX hash cannot be `None`"))
    }
    fn insert(self, cache: &mut HashMap<H256, Tx>) {
        if cache.contains_key(self.hash()) {
            let entry = cache.get_mut(self.hash()).expect("scope is conditional; qed");
            entry.traces = Some(self);
        } else {
            cache.insert(self.hash().clone(), self.empty());
        }
    }

    fn exists(&self, cache: &HashMap<H256, Tx>) -> bool {
        if self.len() <= 0 {
            false
        } else {
            cache.contains_key(self.hash()) && cache.get(self.hash()).expect("scope is conditional; qed").traces.is_some()
        }
    }

    fn empty(self) -> Tx {
        Tx {traces: Some(self), transaction: None, logs: None, receipt: None, block: None}
    }
}

impl CacheAction for Log {
    fn hash(&self) -> &H256 {
        self.transaction_hash.as_ref().expect("Transaction hash cannot be empty")
    }

    fn insert(self, cache: &mut HashMap<H256, Tx>) {
        if cache.contains_key(self.hash()) {
            let entry = cache.get_mut(self.hash()).expect("scope is conditional; qed");
            entry.logs = Some(self);
        } else {
            cache.insert(self.hash().clone(), self.empty());
        }
    }

    fn exists(&self, cache: &HashMap<H256, Tx>) -> bool {
        cache.contains_key(self.hash()) && cache.get(self.hash()).expect("scope is conditional; qed").logs.is_some()
    }
    fn empty(self) -> Tx {
        Tx { logs: Some(self), transaction: None, traces: None, receipt: None, block: None}
    }
}
impl From<Block> for TxType {
    fn from(blk: Block) -> TxType {
        TxType::Block(blk)
    }
}

impl From<Transaction> for TxType {
    fn from(t: Transaction) -> TxType {
        TxType::Transaction(t)
    }
}

impl From<TransactionReceipt> for TxType {
    fn from(tr: TransactionReceipt) -> TxType {
        TxType::Receipt(tr)
    }
}

impl From<Vec<Trace>> for TxType {
    fn from(traces: Vec<Trace>) -> TxType {
        TxType::Traces(traces)
    }
}

impl From<Log> for TxType {
    fn from(log: Log) -> TxType {
        TxType::Logs(log)
    }
}

#[cfg(test)]
mod test {
    use web3::types::Address;
    use super::*;

    #[test]
    fn get_blocknumber() {
        pretty_env_logger::try_init();
        let cache = TransactionCache::new(Address::from("0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359"), BlockNumber::Earliest, BlockNumber::Number(6000000)).unwrap();
        let tx = cache.tx_by_blocknum_index(1165405, 2);
        info!("TX: {:?}", tx);
    }
}

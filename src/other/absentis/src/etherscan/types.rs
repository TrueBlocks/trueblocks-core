use serde_derive::*;
use serde::de::{self, Deserialize, Deserializer};
use web3::types::{H256, U256, Bytes, Address};
use std::fmt::Debug;

// 60,137,282,256
#[derive(Deserialize, PartialEq, Debug, Clone)]
pub struct EtherScanTx {
    #[serde(rename = "blockNumber", deserialize_with = "from_str")]
    pub block_number: u64,
    #[serde(rename = "timeStamp", deserialize_with = "from_str")]
    pub time_stamp: u64,
    pub hash: H256,
    #[serde(deserialize_with = "from_str")]
    pub nonce: u64,
    #[serde(rename = "blockHash")]
    pub block_hash: H256,
    #[serde(rename = "transactionIndex", deserialize_with="from_str")]
    pub transaction_index: usize,
    pub from: Address,
    pub to: String,
    #[serde(deserialize_with="from_str")]
    pub value: U256,
    #[serde(deserialize_with="from_str")]
    pub gas: U256,
    #[serde(rename = "gasPrice", deserialize_with="from_str")]
    pub gas_price: U256,
    #[serde(default)]
    pub is_error: String,
    pub txreceipt_status: String, // no rename needed
    pub input: Bytes,
    #[serde(rename = "contractAddress")]
    pub contract_address: String,
    #[serde(default, rename = "cumulative_gas_used", deserialize_with="from_str")]
    pub cumulative_gas_used: u64,
    #[serde(rename = "gasUsed", deserialize_with="from_str")]
    pub gas_used: u64,
    #[serde(deserialize_with="from_str")]
    pub confirmations: u64,
}

#[derive(Deserialize, Debug)]
pub struct EtherScanInternalTx {
    #[serde(rename = "blockNumber", deserialize_with="from_str")]
    pub block_number: u64,
    #[serde(rename = "timeStamp")]
    pub time_stamp: String,
    pub hash: H256, // we only care about this one
    pub from: String,
    pub to: String,
    pub value: String,
    #[serde(rename = "contractAddress")]
    pub contract_address: String,
    pub input: String,
    #[serde(rename = "type")]
    pub tx_type: String,
    pub gas: String,
    #[serde(rename = "gasUsed")]
    pub gas_used: String,
    #[serde(rename = "traceId")]
    pub trace_id: String,
    #[serde(rename="isError")]
    pub is_error: String,
    #[serde(rename = "errCode")]
    pub err_code: String
}

#[derive(Deserialize, Debug)]
pub struct EtherScanResponse<T: Debug> {
    #[serde(deserialize_with = "from_str")]
    status: i32,
    message: String,
    pub result: T,
}

fn from_str<'de, T, D>(deserializer: D) -> Result<T, D::Error>
where
    T: std::str::FromStr,
    T::Err: std::fmt::Display,
    D: Deserializer<'de>
{
    let s = String::deserialize(deserializer)?;
    T::from_str(&s).map_err(de::Error::custom)
}
// TODO: #p1 these should be functions, but i'm lazy
#[macro_export]
macro_rules! eth_txlist {
    ($addr:expr, $from:expr, $to:expr) => ({
        use crate::types::ETHERSCAN_URL;
        info!("{:x}", $addr)
        &format!("{}?module=account&action=txlist&address=0x{:x}&startblock={}&endblock={}&sort=asc",
                ETHERSCAN_URL,
                $addr,
                $from,
                $to
        )
    });
    ($addr:expr, $from:expr, $to:expr, $sort:expr) => ({
        use crate::types::ETHERSCAN_URL;
        let addr = String::from(serde_json::to_string(&$addr)?);
        info!("ADDR: {}", addr);
        &format!("{}?module=account&action=txlist&address=0x{:x}&startblock={}&endblock={}&sort={}",
                ETHERSCAN_URL,
                $addr,
                $from,
                $to,
                $sort
        )
    });
}

#[macro_export]
macro_rules! eth_int_txlist {
    ($addr:expr, $from:expr, $to:expr) => ({
        use crate::types::ETHERSCAN_URL;
        info!("{:x}", $addr)
            &format!("{}?module=account&action=txlistinternal&address=0x{:x}&startblock={}&endblock={}&sort=asc",
                     ETHERSCAN_URL,
                     $addr,
                     $from,
                     $to
            )
    });
    ($addr:expr, $from:expr, $to:expr, $sort:expr) => ({
        use crate::types::ETHERSCAN_URL;
        let addr = String::from(serde_json::to_string(&$addr)?);
        info!("ADDR: {}", addr);
        &format!("{}?module=account&action=txlistinternal&address=0x{:x}&startblock={}&endblock={}&sort={}",
                 ETHERSCAN_URL,
                 $addr,
                 $from,
                 $to,
                 $sort
        )
    });
}

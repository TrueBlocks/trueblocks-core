use log::*;
#[macro_use] mod types;
pub use self::types::{EtherScanTx, EtherScanInternalTx, EtherScanResponse};
use crate::err::{ErrorKind};
use hyper::client::HttpConnector;
use itertools::Itertools;
use futures::{
    future::Future,
    stream::Stream,
};
use failure::{Error, ResultExt};
use web3::types::{H160, H256};

pub struct EtherScan {
    client: hyper::client::Client<HttpConnector, hyper::Body>,
}

pub enum SortType {
    Ascending,
    Descending,
    None
}

impl From<&SortType> for String {
    fn from(sort_type: &SortType) -> String {
        match sort_type {
            SortType::Ascending => "asc".to_string(),
            SortType::Descending => "des".to_string(),
            SortType::None => "asc".to_string()
        }
    }
}


impl EtherScan {
    pub fn new() -> Self {
        EtherScan {
            client: hyper::client::Client::new()
        }
    }

    /// returns all hashes of transactions (external  + internal)
    pub fn get_tx_by_account(&self, ev_loop: &mut tokio_core::reactor::Core,
                             addr: H160,
                             from: u64,
                             to: u64,
                             sort: SortType)
                             -> Result<Vec<(H256, u64)>, Error>
    {
        let req_normtx = eth_txlist!(addr, from.to_string(), to.to_string(), String::from(&sort));
        let req_inttx = eth_int_txlist!(addr, from.to_string(), to.to_string(), String::from(&sort));
        info!("URL: {}::::{}", req_normtx, req_inttx);
        let normal =  ev_loop.run(self.do_get(req_normtx.parse().expect("URI should not be invalid; qed")))?;
        let internal = ev_loop.run(self.do_get(req_inttx.parse().expect("URI should not be invalid; qed")))?;
        let norm_response = serde_json::from_slice::<EtherScanResponse<Vec<EtherScanTx>>>(&normal.to_vec())?.result;
        let int_response = serde_json::from_slice::<EtherScanResponse<Vec<EtherScanInternalTx>>>(&internal.to_vec()).context(ErrorKind::Parse)?.result;
        Ok(norm_response
           .iter()
           .map(|x| (x.hash, x.block_number))
           .chain(int_response.iter().map(|x| (x.hash, x.block_number)))
           .into_iter()
           .unique_by(|x| x.0)
           .collect::<Vec<(H256, u64)>>())
    }

    fn do_get(&self, uri: hyper::Uri) -> impl Future<Item = bytes::Bytes, Error = Error> {
        self.client
            .get(uri)
            .and_then(|res| {
                assert_eq!(res.status(), hyper::StatusCode::OK);
                res.into_body().concat2()
            })
            .map_err(|e| ErrorKind::Network(format!("{}", e)).into())
            .and_then(|json| {
                futures::future::result(Ok(json.into_bytes()))
            })
    }
}

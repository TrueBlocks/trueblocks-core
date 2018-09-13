//! parses configuration file and CLI options to return configured values
mod config_file;
mod cli;

use log::{error, warn};
use failure::Error;
use std::path::PathBuf;
use web3::{
    transports::{
        http::Http,
        ipc::Ipc,
    }
};

use self::config_file::{ConfigFile, Transport as Transport};
use super::client::Client;
use super::err::{ErrorKind, ConfMsg};

pub use self::cli::Action;

pub struct Configuration {
    file: Option<ConfigFile>,
    log_level: LogLevel,
    url: String,
    transport: Transport,
    pub action: Action,
}

pub enum ChosenClient {
    Http(Client<Http>),
    Ipc(Client<Ipc>),
    Infura,
}

#[derive(Debug)]
pub enum LogLevel {
    None,   // Error by default
    Pleasant, // info's/warns
    Tolerable, // debug
    InsaneMode // trace/debug/info/warns
}

impl Configuration {

    pub fn new() -> Result<Self, Error> {
        let opts = self::cli::parse()?;
        let (file, url, transport) = url_or_file(opts.file, opts.url, opts.transport)?;
        let action = opts.action;
        Ok(Configuration {
            file, url, transport, action,
            log_level: opts.log_level,
        })
    }

    // get a configured client
    pub fn get_client(&self) -> Result<ChosenClient, Error> {
        match self.transport {
            Transport::Http => Ok(ChosenClient::Http(Client::<Http>::new_http(self)?)),
            Transport::Ipc => Ok(ChosenClient::Ipc(Client::<Ipc>::new_ipc(self)?)),
            _ => unimplemented!(),
        }
    }

    pub fn url(&self) -> String {
        self.url.clone()
    }

    pub fn ipc_path(&self) -> PathBuf {
        PathBuf::from(&self.url)
    }
}

// TODO: #p3
// if a host is not specific, we use the first host to respond to a Http JsonRPC net_peers query
// if no hosts are found, we warn the user that no hosts have been found, and are falling back to
// infura
// if infura api key is not found, we inform the user and exit
// fn get_node(file: &ConfigFile) -> EthNode {
//     // first, ping the host to make sure they are even up
//     // then depending on IPC or Http, create a synchronous web3 client,
//     // return first host to respond to netPeers
//     unimplemented!();
// }

/// parse cli arguments and/or configuration file if specified/default
type UrlOrFile = (Option<ConfigFile>, String, Transport);
fn url_or_file(file: Option<ConfigFile>, url: Option<String>, transport: Option<Transport>)
               -> Result<UrlOrFile, Error> {
    match (file, url) {
        (None, Some(url)) => {
            let transport = transport.expect("A single url means `node` or `infura` was used");
            Ok((None, url, transport))
        },
        (Some(file), None) => {
            let default = file.default_ident();
            let url_info = file.transport(None, |node| node.matches(&default));
            if url_info.is_err() { // if we couldn't find a node to use, fallback to Infura
                error!("{}", url_info.expect_err("scope is conditional; qed"));
                warn!("Could not find a node to use based on default identifier. \
                       attempting to fall back to Infura");
                let url = file.infura_url()?;
                let transport = Transport::Infura;
                Ok((Some(file), url, transport))
            } else {
                let (url, transport) = url_info.expect("Scope is conditional; qed");
                Ok((Some(file), url, transport))
            }
        },
        (f @ Some(_), Some(url)) => {
            // everything is specified
            let transport = transport.expect("Everything specified; qed");
            Ok((f, url, transport))
        }
        _ => {
            error!("{}", verb_msg!("Must specify one of file or url"));
            Err(ErrorKind::InvalidConfiguration(ConfMsg::NotFound("Valid Configuration".to_string())).into())
        }
    }
}

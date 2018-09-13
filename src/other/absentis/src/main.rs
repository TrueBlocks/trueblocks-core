#![feature(transpose_result, test)]
#[macro_use] mod utils;
mod types;
mod conf;
// mod node;
mod err;
mod client;
// mod transaction_finder;
mod transaction_validator;
// mod filter;
mod etherscan;
use failure::Error;

use self::conf::{ChosenClient, Action};
use web3::{
    types::{BlockNumber, H160},
    BatchTransport
};
use futures::stream::Stream;
use self::client::Client;
use std::path::PathBuf;

// TODO SOMETIME BEFORE RELEASE
//  - make errors nice and not sloppy

fn main() -> Result<(), Error>{
    pretty_env_logger::init();
    let conf = conf::Configuration::new()?;
    let client = conf.get_client()?;
    match conf.action {
        Action::Validate{file, to, address} => {
            match client {
                ChosenClient::Http(c) => do_validate(c, to, address, file)?,
                ChosenClient::Ipc(c) => do_validate(c, to, address, file)?,
                _ => unimplemented!()
            }
        }
    }
    Ok(())
}

fn do_validate<T>(mut client: Client<T>, to: Option<u64>, address: H160, file: PathBuf) -> Result<(), Error>
where
    T: BatchTransport + Send + Sync + 'static,
    <T as web3::Transport>::Out: Send
{
    let to = to.map(|t| BlockNumber::Number(t));
    let fut = self::transaction_validator::TransactionValidator::new(&mut client, file, to, address)?
        .scan(&client)?
        .for_each(|v| {
            println!("{}", v);
            Ok(())
        });
    client.run(fut).unwrap();
    Ok(())
}

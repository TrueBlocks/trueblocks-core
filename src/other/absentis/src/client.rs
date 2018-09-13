use log::*;
use std::path::PathBuf;
use web3::{Transport, BatchTransport, transports};
use failure::{Error, ResultExt};
use futures::future::Future;
use super::{
    types::MAX_PARALLEL_REQUESTS,
    conf::Configuration,
    err::ErrorKind
};

pub struct Client<T: Transport> where web3::transports::batch::Batch<T>: Transport {
    pub web3: web3::Web3<T>,
    pub web3_batch: web3::Web3<web3::transports::batch::Batch<T>>,
    transport: T,
    ev_loop: tokio_core::reactor::Core,
}

impl<T> Client<T> where T: BatchTransport + Clone {

    pub fn new(transport: T) -> Result<Self, Error> {
        let ev_loop = tokio_core::reactor::Core::new().context(ErrorKind::Async)?;
        Ok(Client {
            web3: web3::Web3::new(transport.clone()),
            web3_batch: web3::Web3::new(web3::transports::Batch::new(transport.clone())),
            transport, ev_loop,
        })
    }

    pub fn remote(&self) -> tokio_core::reactor::Remote {
        self.ev_loop.remote()
    }

    pub fn handle(&self) -> tokio_core::reactor::Handle {
        self.ev_loop.handle()
    }

    pub fn turn(&mut self) -> () {
        loop {
            self.ev_loop.turn(None);
        }
    }

    pub fn run<R, E>(&mut self, fut: impl Future<Item=R, Error=E>) -> Result<R, E> {
        self.ev_loop.run(fut)
    }

    pub fn ev_loop(&mut self) -> &mut tokio_core::reactor::Core {
        &mut self.ev_loop
    }

    /// returns a new web3_batch instance. useful for separating out types of requests
    pub fn batch(&self) -> web3::Web3<web3::transports::batch::Batch<T>> {
        web3::Web3::new(web3::transports::Batch::new(self.transport.clone()))
    }

    pub fn new_ipc(conf: &Configuration) -> Result<Client<transports::ipc::Ipc>, Error> {
        let ev_loop = tokio_core::reactor::Core::new().context(ErrorKind::Async)?;
        ClientBuilder::ipc()
            .path(PathBuf::from(conf.url()))
            .handle(ev_loop.handle())
            .build(ev_loop)
            .map_err(|e| e.into())
    }

    pub fn new_http(conf: &Configuration) -> Result<Client<transports::http::Http>, Error> {
        let ev_loop = tokio_core::reactor::Core::new().context(ErrorKind::Async)?;
        ClientBuilder::http()
           .url(conf.url())
           .handle(ev_loop.handle())
           .build(ev_loop)
           .map_err(|e| e.into())
    }
}

struct HttpBuilder {
    url: Option<String>,
    max_parallel: Option<usize>,
    handle: Option<tokio_core::reactor::Handle>,
}

impl HttpBuilder {
    fn url(&mut self, val: String) -> &mut Self {
        let new = self;
        new.url = Some(val);
        new
    }
    fn max_parallel(&mut self, val: usize) -> &mut Self {
        let new = self;
        new.max_parallel = Some(val);
        new
    }
    fn handle(&mut self, val: tokio_core::reactor::Handle) -> &mut Self {
        let new = self;
        new.handle = Some(val);
        new
    }

    fn build(&self, ev_loop: tokio_core::reactor::Core) ->  Result<Client<transports::http::Http>, Error> {
        let url = self.url.as_ref().ok_or(ErrorKind::Internal)?;
        let handle = self.handle.as_ref().ok_or(ErrorKind::Internal)?;
        let max = self.max_parallel.unwrap_or(MAX_PARALLEL_REQUESTS);
        let http =
            web3::transports::Http::with_event_loop(url,handle,max);

        let http = match http {
            Ok(v) => v,
            Err(e) => {
                pretty_err!("{}{}", "Could not initialize the Web3 Object: ", e.description());
                if let Some(bt) = e.backtrace() {
                    // pretty_err!("{}{}", "Backtrace: ", bt);
                    println!("Backtrace: {:?}", bt);
                }
                panic!("Shutting down...");
            }
        };


        Ok(Client {
            web3: web3::Web3::new(http.clone()),
            web3_batch: web3::Web3::new(web3::transports::batch::Batch::new(http.clone())),
            transport: http,
            ev_loop,
        })
    }
}

struct IpcBuilder {
    path: Option<std::path::PathBuf>,
    handle: Option<tokio_core::reactor::Handle>,
}

impl IpcBuilder {
    fn path(&mut self, path: PathBuf) -> &mut Self {
        let new = self;
        new.path = Some(path);
        new
    }
    fn handle(&mut self, handle: tokio_core::reactor::Handle) -> &mut Self {
        let new = self;
        new.handle = Some(handle);
        new
    }
    fn build(&self, ev_loop: tokio_core::reactor::Core) -> Result<Client<transports::ipc::Ipc>, Error> {
        let path = self.path.as_ref().ok_or(ErrorKind::Internal)?;
        let handle = self.handle.as_ref().ok_or(ErrorKind::Internal)?;
        let ipc = web3::transports::Ipc::with_event_loop(path.as_path(), handle);

        let ipc = match ipc {
            Ok(v) => v,
            Err(e) => {
                pretty_err!("{}{}", "Could not initialize the Web3 Object: ", e.description());
                if let Some(bt) = e.backtrace() {
                    // pretty_err!("{}{}", "Backtrace: ", bt);
                    println!("Backtrace: {:?}", bt);
                }
                panic!("Shutting down...");
            }
        };

        Ok(Client {
            web3: web3::Web3::new(ipc.clone()),
            web3_batch: web3::Web3::new(web3::transports::batch::Batch::new(ipc.clone())),
            transport: ipc,
            ev_loop,
        })
    }
}

#[derive(Default)]
pub struct ClientBuilder;


impl ClientBuilder {
    fn ipc() -> IpcBuilder {
        IpcBuilder {
            path: None,
            handle: None
        }
    }

    fn http() -> HttpBuilder {
        HttpBuilder {
            url: None,
            max_parallel: None,
            handle: None,
        }
    }
}

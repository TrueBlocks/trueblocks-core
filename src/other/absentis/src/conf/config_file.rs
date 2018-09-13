// macros
use serde_derive::*;
use log::*;

use failure::{Error, ResultExt};
use std::{ fs, env, io::Write, path::PathBuf};
use config::{File, Config};
use reduce::Reduce;
use clap::{arg_enum, _clap_count_exprs};
use crate::err::{ErrorKind, ConfMsg};

#[derive(Serialize, Deserialize, Debug)]
pub struct ConfigFile {
    default: String, // identifier for default node to use
    nodes: Option<Vec<EthNode>>,
    infura: Option<Infura>,
}

arg_enum! { // allows for automatic deser of cli args into enum
    #[derive(Serialize, Deserialize, Debug, Clone)]
    pub enum Transport {
        Http,
        Ipc,
        Infura
    }
}
impl Default for Transport {
    fn default() -> Transport {
        Transport::Http
    }
}

#[derive(Serialize, Deserialize, Debug)]
pub struct EthNode {
    #[serde(rename = "identifier")]
    ident: String,
    #[serde(default)]
    transport: Transport,
    http: Option<Http>,
    ipc: Option<Ipc>
}

impl std::fmt::Display for EthNode {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(f, "{}", self.ident)
    }
}

impl EthNode {
    pub fn matches(&self, ident: &str) -> bool {
        self.ident.to_lowercase() == ident.to_lowercase()
    }
}

#[derive(Serialize, Deserialize, Debug)]
struct Http {
    url: String,
    port: usize,
}

impl Http {
    fn url(&self) -> String {
        format!("{}:{}", self.url, self.port)
    }
}

#[derive(Serialize, Deserialize, Debug)]
struct Ipc {
    path: String
}

impl Ipc {
    fn path_str(&self) -> String {
        self.path.clone()
    }

    fn path(&self) -> PathBuf {
        PathBuf::from(self.path.clone())
    }
}

#[derive(Serialize, Deserialize, Debug)]
struct Infura {
    api_key: String,
}


impl Default for ConfigFile {

    fn default() -> Self {

        let mut nodes: Vec<EthNode> = Vec::new();
        nodes.push(EthNode {
            ident: "Parity".to_string(),
            http: Some(Http {
                url: "http://localhost".to_string(),
                port: 8545 as usize
            }),
            ipc: None,
            transport: Transport::Http,
        });

        let infura = Some(Infura {
            api_key: "".to_string(),
        });

        ConfigFile {
            nodes: Some(nodes),
            infura,
            default: "Parity".to_string()
        }
    }
}

// file creation
impl ConfigFile {
    /// Default configuration path is ~/.config/absentis.toml (On UNIX)
    /// this can be modified by passing -c (--config) to absentis
    pub fn new(mut config_path: Option<PathBuf>) -> Result<Self, Error> {
        let mut tmp = env::temp_dir();
        tmp.push("absentis_default.toml");
        info!("Temp Config Path: {:?}", &tmp);
        let mut default_file = fs::File::create(tmp.clone()).with_context(|e| format!("Could not create temporary configuration: {}", e))?;
        let default_config = Self::default();
        let toml = toml::to_string_pretty(&default_config).with_context(|e| format!("Could not serialize default configuration {}", e))?;
        default_file.write_all(toml.as_bytes()).with_context(|e| format!("Could not write to temporary default configuration {}", e))?;
        info!("Default ConfigFile: {:?}", default_config);

        if config_path.is_none() { // if a custom configuration path has not been set, use default
            config_path = Some(Self::default_path().and_then(|p| {
                if !p.as_path().exists() { // check to make sure the user config exists,
                    // if not create an empty file so we can fill it with defaults
                    let mut new_f = fs::File::create(p.as_path()).with_context(|e| format!("Could not create new config {}", e))?;
                    new_f.write_all(toml.as_bytes()).with_context(|e| format!("Could not write to new config {}", e))?;
                }
                Ok(p)
            })?);
        }
        let mut conf = Config::new();
        conf.merge(File::with_name(tmp.to_str().expect("Temp file should always be valid UTF-8")))
            .context(ErrorKind::InvalidConfiguration(ConfMsg::None))?;
        conf.merge(
                File::with_name(config_path.expect("scope is conditional; qed")
                                .to_str().expect("Should always be valid UTF-8"))
            ).context(ErrorKind::InvalidConfiguration(ConfMsg::None))?;
        conf.try_into().map_err(|e| e.into())
    }

    pub fn new_default() -> Result<(), Error> {
        let default_config = Self::default();
        let toml = toml::to_string_pretty(&default_config).context(ErrorKind::InvalidConfiguration(ConfMsg::None))?;
        let default_path = Self::default_path().context(ErrorKind::InvalidConfiguration(ConfMsg::None))?;
        if !default_path.as_path().exists() { // check to see if a default already exists
            let mut new_default = fs::File::create(default_path.as_path()).context(ErrorKind::InvalidConfiguration(ConfMsg::None))?;
            new_default.write_all(toml.as_bytes()).context(ErrorKind::InvalidConfiguration(ConfMsg::None))?;
            Ok(())
        } else {
            warn!("Default already exists");
            Ok(())
        }
    }

    pub fn from_default() -> Result<ConfigFile, Error> {
        let path = Self::default_path().context(ErrorKind::InvalidConfiguration(ConfMsg::None))?;
        Ok(fs::read_to_string(path.as_path())
            .context(ErrorKind::InvalidConfiguration(ConfMsg::None))?
            .parse()
            .context(ErrorKind::InvalidConfiguration(ConfMsg::None))?)
    }

    pub fn from_custom(config_path: PathBuf) -> Result<ConfigFile, Error> {
        Ok(fs::read_to_string(config_path.as_path())
            .context(ErrorKind::InvalidConfiguration(ConfMsg::None))?
            .parse()
            .context(ErrorKind::InvalidConfiguration(ConfMsg::None))?)
    }

    pub fn default_exists() -> bool {
        match Self::default_path() {
            Err(e) => {
                warn!("{}", e);
                false
            },
            Ok(v) => v.as_path().exists()
        }
    }

    fn default_path() -> Result<PathBuf, Error> {
        dirs::config_dir().and_then(|mut conf| {
            conf.push("absentis.toml");
            Some(conf)
        }).ok_or(ErrorKind::InvalidConfiguration(ConfMsg::None).into())
    }
}

macro_rules! is_set {
    ($opt:expr, $msg:expr) => ({
        $opt.ok_or_else(|| ErrorKind::InvalidConfiguration(ConfMsg::OptionNotSet($msg.to_string())))?
    });
}

macro_rules! is_found {
    ($opt:expr, $msg:expr) => ({
        $opt.ok_or_else(|| ErrorKind::InvalidConfiguration(ConfMsg::NotFound($msg.to_string())))?
    })
}

// getters
impl ConfigFile {

    pub fn infura_url(&self) -> Result<String, Error> {
        Ok(infura_url!(self.infura_key()?))
    }

    fn infura_key(&self) -> Result<String, Error>  {
        let inf = is_set!(self.infura.as_ref(), "Infura Api Key");
        Ok(inf.api_key.clone())
    }

    pub fn default_ident(&self) -> &String { // the default node
        &self.default
    }

    // gets an ethNode transport based on the predicate function and transport(if specified)
    // if transport is not specified, returns default from file
    // Transport is returned as a String. So, if the transport is IPC it will have to be converted
    // to a Path
    pub fn transport<F>(&self, transport: Option<Transport>, fun: F)
        -> Result<(String, Transport), Error>
        where
            F: Fn(&EthNode) -> bool
    {
        let nodes = is_set!(self.nodes.as_ref(), "Nodes");
        let node: Option<&EthNode> = nodes.iter().filter(|x| fun(x)).take(1).reduce(|e, _| e);
        let node = is_found!(node, "Eth node with identifier");

        if let Some(trans) = transport {
            match trans {
                t @ Transport::Http => {
                    let url =
                        is_set!(node.http.as_ref(), format!("Http for node {}", node.ident))
                        .url();
                    Ok((url, t))
                },
                t @ Transport::Ipc => {
                    let url =
                        is_set!(node.ipc.as_ref(), format!("Ipc for node {}", node.ident))
                        .path_str();
                    Ok((url, t))
                },
                t @ Transport::Infura => {
                    is_set!(self.infura.as_ref(), "Infura API Key");
                    let url = self.infura_url()?;
                    Ok((url, t))
                }
            }
        } else {
            match &node.transport {
                t @ Transport::Http => {
                    let url =
                        is_set!(node.http.as_ref(), format!("Http for node {}", node.ident))
                        .url();
                    Ok((url, t.clone()))
                },
                t @ Transport::Ipc => {
                    let url =
                        is_set!(node.ipc.as_ref(), format!("Ipc for node {}", node.ident))
                        .path_str();
                    Ok((url, t.clone()))
                },                          // change the name to something else for arbitrary net JSONRPC nodes
                t @ Transport::Infura => { // TODO: This really shouldn't be allowed, #p3
                    is_set!(self.infura.as_ref(), "Infura API Key");
                    let url = self.infura_url()?;
                    Ok((url, t.clone()))
                }
            }
        }
    }

    // returns the url from the first Eth node that matches the predicate function
    pub fn url<F>(&self, fun: F) -> Result<String, Error>
        where
            F: Fn(&EthNode) -> bool
    {
        let nodes = is_set!(self.nodes.as_ref(), "Eth Nodes");
        let node: Option<&EthNode> = nodes.iter().filter(|x| fun(x)).take(1).reduce(|e, _| e);


        // panic because predicate entered should never be incorrect
        // that would be an internal bug
        let node = node.expect("Predicate entered should never be incorrect; qed");

        let http: &Http = is_set!(node.http.as_ref(), format!("Http info for node {}", node));
        Ok(http.url())
    }

    // returns the ipc path from the first EthNode that matches the predicate function
    pub fn ipc_path<F>(&self, fun: F) -> Result<PathBuf, Error>
        where
            F: Fn(&EthNode) -> bool
    {
        let nodes = is_set!(self.nodes.as_ref(), "Eth Nodes");
        let node: Option<&EthNode> = nodes.iter().filter(|x| fun(x)).take(1).reduce(|e, _| e);

        // panic because predicate entered should never be incorrect
        // that would be an internal bug
        let node = node.expect("Predicate entered should never be incorrect; qed");

        let ipc: &Ipc = is_set!(node.ipc.as_ref(), format!("IPC info for node {}", node));

        Ok(ipc.path())
    }
}

pub trait Parse {
    fn parse(&self) -> Result<ConfigFile, toml::de::Error>;
}

impl Parse for String {
    fn parse(&self) -> Result<ConfigFile, toml::de::Error> {
        toml::from_str(self)
    }
}


#[cfg(test)]
mod tests {
    use super::*;
    use log::{debug, error, info, log};
    use pretty_env_logger;
    // this test tends to screw things up

    #[test]
    fn it_should_return_default_path() {
        pretty_env_logger::try_init();
        let path = ConfigFile::default_path();
        let path = match path {
            Ok(p) => p,
            Err(e) => {
                error!("Error in test: {}", e);
                panic!("Failed due to error");
            }
        };
        // TODO: change to make general test #p2
        assert_eq!(path.to_str().unwrap(), "/home/insi/.config/absentis.toml");
    }

    #[test]
    fn it_should_return_config_from_default_path() {
        pretty_env_logger::try_init();
        let conf = ConfigFile::from_default();
        match conf {
            Ok(c) =>  {
                info!("Config: {:?}", c);
            },
            Err(e) => {
                error!("Error in test: {}", e);
                panic!("Failed due to error");
            }
        }
    }
}

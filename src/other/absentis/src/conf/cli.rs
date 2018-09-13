use log::*;
use std::path::PathBuf;
use std::str::FromStr;
use web3::types::{Address};
use clap::{App, load_yaml, value_t};
use colored::Colorize;
use failure::Error;
use super::{
    LogLevel,
    config_file::{ConfigFile, Transport},
};

#[derive(Debug, Clone)]
pub enum Action {
    Validate{file: PathBuf, to: Option<u64>, address: Address},
}

pub struct CLIArgs {
    pub file: Option<ConfigFile>,
    pub url: Option<String>,
    pub transport: Option<Transport>,
    pub log_level: LogLevel,
    pub action: Action,
}

pub fn parse() -> Result<CLIArgs, Error> {
    let yaml = load_yaml!("cli_args.yml");
    let matches = App::from_yaml(yaml).get_matches();
    let mut file: Option<ConfigFile> = None;
    let mut url: Option<String> = None;
    let mut transport: Option<Transport> = None;

    let log_level = match matches.occurrences_of("verbose") {
        0 => LogLevel::None,
        1 => LogLevel::Pleasant,
        2 => LogLevel::Tolerable,
        3 => LogLevel::InsaneMode,
        4 | _ => {
            print!("{}", "You're beyond Insane:".bright_red().bold().on_black());
            println!("{}", " Insane mode activated".magenta().blink());
            LogLevel::InsaneMode
        }
    };

    if matches.is_present("generate_config") {
        ConfigFile::new_default()?;
        info!("Configuration Generated!");
    }

    if let Some(custom_config) = matches.value_of("config") {
        file = Some(ConfigFile::from_custom(PathBuf::from(custom_config))?);
    } else if ConfigFile::default_exists() {
        file = Some(ConfigFile::from_default()?)
    }

    transport = value_t!(matches.value_of("transport"), Transport).ok();

    if matches.value_of("identify").is_some() && file.is_some() {
        let file = file.as_ref().expect("Scope is conditional; qed");
        let ident = matches.value_of("identify").expect("Scope is conditional; qed");
        if ident == "Infura".to_lowercase() {
            url = Some(file.infura_url()?);
            transport = Some(Transport::Infura);
        } else {
            let trans = file.transport(transport, |node| node.matches(ident))?;
            url = Some(trans.0);
            transport = Some(trans.1);
        }
    } else if matches.value_of("identify").is_some()  && file.is_none() {
        error!("Cannot identify node if configuration file is not specified or does not exist");
        std::process::exit(1);
    }

    if let Some(infura_key) = matches.value_of("infura") {
        url = Some(infura_url!(infura_key));
        transport = Some(Transport::Infura)
    }

    if let Some(node) = matches.value_of("node") {
        url = Some(node.to_owned());
        transport = Some(transport.expect("transport is conditional on node; qed"));
    }

    if file.is_none() && url.is_none() {
        error!("Need a configuration file or a node url/w transport specified to proceed");
        std::process::exit(1);
    }
    let mut action = None;
    if let Some(file) = matches.value_of("csv_validate") {
        let to = matches.value_of("to").map(|t| t.parse()).transpose()?;
        let address = matches.value_of("address").unwrap();
        action = Some(Action::Validate{file: PathBuf::from(file), to: to, address: Address::from_str(address)?})
    }
    // possible options:
    // Url + Transport
    // Config file specified w/ no transport specified (use default from file)
    // Config file specified with node identified but not transport specified (use default from file)
    // Config file specified with node identified and transport chosen
    // Infura key specified (got url)
    // node specified, url+transport guaranteed
    if action.is_none() {
        error!("Need to specify an action");
        std::process::exit(1);
    }
    let action = action.unwrap();

    Ok(CLIArgs {
        file, url, transport, log_level, action
    })
}

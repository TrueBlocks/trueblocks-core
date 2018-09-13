use web3::BatchTransport;
use futures::future::Future;
use super::client::Client;
/*
macro_rules! replace_expr {
    ($_t:tt $sub:expr) => {$sub};
}

macro_rules! count_tts {
    ($($tts:tt)*) => {0usize $(+ replace_expr!($tts 1usize))*};
}
*/
/*macro_rules! green {
    ($($strs: expr),+) => ({
        use colored::Colorize;
        $($strs.bright_green()),+
    });
}*/
#[macro_export]
macro_rules! format_num {
    ($num:expr) => ({
        use separator::Separatable;
        $num.separated_string()
    });
}

#[macro_export]
macro_rules! pretty_err {
    // colors entire string red underline and bold
    /*($format:expr, $($str:expr),+) => ({
        use colored::Colorize;
        format!($format, $($str.red().bold().underline()),+)
    }); */
    //colors first string bright red, bold, underline, rest dimmed
    ($format:expr, $str:expr, $($muted:expr),*) => ({
        use log::error;
        use colored::Colorize;
        let string = format!($format, $str.bright_red().bold().underline(), $($muted.red().dimmed()),+);
        error!("{}", string);
    });
}

#[macro_export]
macro_rules! verb_msg {
    ($msg: expr) => ({
        format!("{} on line: {}, col: {}, in file: {}", $msg, line!(), column!(), file!())
    });
    ($msg: expr, $( $frmt:expr ),*) => ({
        let string = format!($msg, $($frmt),*);
        format!("{} on line: {}, col: {}, in file: {}", string, line!(), column!(), file!())
    })
}

#[macro_export]
macro_rules! pretty_info {
    ($frmt:expr, $($strs:expr),+) => ({
        let string = format!($frmt, $(green!($strs)),+);
        info!("{}", string);
    });
}


#[macro_export]
macro_rules! infura_url {
    ($api_key:expr) => ({
        use crate::types::INFURA_URL;
        format!("{}{}", INFURA_URL, $api_key)
    });
}

#[macro_export]
macro_rules! panic_web3 {
    ($web3:expr) => ({
        pretty_err!("{}, {}", "Web3 Error submitting batch!", format!("{}", $web3));
        panic!("Shutting down due to web3 error in: {}, {}, {}", line!(), column!(), file!());
    });
}

#[macro_export]
macro_rules! try_web3 {
    ($web3:expr) => ({
        match $web3 {
            Err(e) => panic_web3!(e),
            Ok(v) => v,
        }
    });
}

pub fn latest_block<T>(client: &Client<T>) -> u64
where
    T: BatchTransport
{
    let b = client.web3.eth().block_number().wait();
    let b = match b {
        Ok(v) => v,
        Err(e) => {
            pretty_err!("{}{}", "Could not get latest block: ", e.description());
            if let Some(bt) = e.backtrace() {
                pretty_err!("{}: {:?}", "Backtrace", format!("{:?}", bt));
            }
            panic!("Shutting down...");
        }

    };
    b.as_u64()
}

pub fn as_u64<T>(client: &Client<T>, block: web3::types::BlockNumber) -> u64
where
    T: BatchTransport
{
    match block {
        web3::types::BlockNumber::Earliest => 0 as u64,
        web3::types::BlockNumber::Latest => latest_block(client),
        web3::types::BlockNumber::Pending => latest_block(client),
        web3::types::BlockNumber::Number(num)=> num,
    }
}

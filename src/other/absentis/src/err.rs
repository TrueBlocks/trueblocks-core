use failure::{Fail, Context, Backtrace};
use std::fmt::{self, Display, Formatter};

#[derive(Debug)]
pub struct AbsentisError {
    inner: Context<ErrorKind>,
}

#[derive(Clone, Eq, PartialEq, Debug, Fail)]
pub enum ErrorKind {
    #[fail(display = "Network Error")]
    Network(String),
    #[fail(display = "Failure parsing response from Network")]
    Parse,
    #[fail(display = "Error while attempting asynchronous action")]
    Async,
    #[fail(display = "Error while parsing command-line arguments")]
    CLI,
    #[fail(display = "Configuration (toml) is invalid or could not be found")]
    InvalidConfiguration(ConfMsg),
    #[fail(display = "Error while attempting to load transactions from database")]
    Database,
    #[fail(display = "Error interacting with cache")]
    Cache,
    #[fail(display = "Error validating transactions")]
    Validate(ValidateMsg),
    #[fail(display = "An Internal Error has occurred. Please File a Bug Report.")]
    Internal,
}

#[derive(Clone, Eq, PartialEq, Debug, Fail)]
pub enum ValidateMsg {
    #[fail(display = "CSV could not be parsed; invalid data")]
    InvalidCsv
}

#[derive(Clone, Eq, PartialEq, Debug, Fail)]
pub enum ConfMsg {
    #[fail(display = "")]
    None,
    #[fail(display = "Option {} not set", _0)]
    OptionNotSet(String),
    #[fail(display = "Could not find {}", _0)]
    NotFound(String)
}

impl Fail for AbsentisError {
    fn cause(&self) -> Option<&Fail> {
        self.inner.cause()
    }
    fn backtrace(&self) -> Option<&Backtrace> {
        self.inner.backtrace()
    }
}

impl Display for AbsentisError {
    fn fmt(&self, f: &mut Formatter) -> fmt::Result {
        Display::fmt(&self.inner, f)
    }
}

impl AbsentisError {
    pub fn kind(&self) -> &ErrorKind {
        &*self.inner.get_context()
    }
}

impl From<ErrorKind> for AbsentisError {
    fn from(kind: ErrorKind) -> AbsentisError {
        AbsentisError { inner: Context::new(kind) }
    }
}

impl From<Context<ErrorKind>> for AbsentisError {
    fn from(inner: Context<ErrorKind>) -> AbsentisError {
        AbsentisError { inner }
    }
}

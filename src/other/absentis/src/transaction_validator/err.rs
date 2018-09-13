use failure::Fail;

#[derive(Debug, Fail)]
pub enum CacheError {
    #[fail(display = "Could not find {}", _0)]
    NotFound(String),
    #[fail(display = "IO Error, {}", _0)]
    IO(#[fail(cause)] std::io::Error),
}

impl From<std::io::Error> for CacheError {
    fn from(err: std::io::Error) -> CacheError {
        CacheError::IO(err)
    }
}

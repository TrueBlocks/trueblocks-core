//! a simple database that serializes to/from messagepack
use log::*;
use std::{
    io::prelude::*,
    io::SeekFrom,
    fs::{self, File, OpenOptions},
    marker::PhantomData,
    path::PathBuf,
    default::Default,
};
use flate2::{
    Compression,
    write::DeflateEncoder,
    read::DeflateDecoder,
};
use serde::{
    Serialize,
    de::DeserializeOwned
};
use failure::{Error, ResultExt};
use crate::err::{ErrorKind};

#[derive(Debug)]
crate struct SimpleDB<D: DeserializeOwned + Serialize + Default> {
    path: PathBuf,
    _marker: PhantomData<D>,
}
// TODO: Figure out a way to use MessagePack instead of JSON
// JSON is OK because we compress it
// compression bench: of ETH tipjar addr txs, block 0-6mil - uncompressed 100MB, compressed 3.9MB
/// A simple DB that allows saving/retrieving structures to/from a (compressed) file,
impl<D> SimpleDB<D> where D: DeserializeOwned + Serialize + Default {
    crate fn new(path: PathBuf) -> Result<Self, Error> {
        if !path.as_path().exists() {
            File::create(path.as_path()).context(ErrorKind::Database)?;
        }
        Ok(SimpleDB {
            path,
            _marker: PhantomData
        })
    }

    /// Save structure to a file, serializing to JSON and then compressing with DEFLATE
    crate fn save(&self, data: D) -> Result<(), Error> {
        self.mutate(|file| {
            let ser_data = serde_json::ser::to_vec(&data).context(ErrorKind::Database)?;
            let mut e = DeflateEncoder::new(file, Compression::default());
            e.write_all(ser_data.as_slice()).context(ErrorKind::Database)?;
            e.finish().context(ErrorKind::Database)?;
            Ok(())
        }).context(ErrorKind::Database)?;
        Ok(())
    }

    /// Get structure from file, DEFLATING and then deserializing from JSON
    crate fn get(&self) -> Result<D, Error> {
        let meta = fs::metadata(self.path.as_path()).context(ErrorKind::Database)?;
        if meta.len() == 0 {
            info!("File length is 0");
            return Ok(D::default());
        }
        self.read(|file| {
            let mut deflater = DeflateDecoder::new(file);
            let mut s = String::new();
            let bytes_read = deflater.read_to_string(&mut s).context(ErrorKind::Database)?;
            info!("Read {} bytes from database file", bytes_read);
            Ok(serde_json::de::from_str(&s)?)
        })
    }

    /// open backend
    fn open(&self) -> Result<File, Error> {
        Ok(OpenOptions::new().create(true).read(true).write(true).open(self.path.as_path()).context(ErrorKind::Database)?)
    }

    /// mutate the file, always setting seek back to beginning
    fn mutate<F>(&self, mut fun: F) -> Result<(), Error>
    where
        F: FnMut(&mut File) -> Result<(), Error>
    {
        let mut file = self.open().context(ErrorKind::Database)?;
        fun(&mut file).context(ErrorKind::Database)?;
        file.seek(SeekFrom::Start(0)).context(ErrorKind::Database)?;
        Ok(())
    }

    /// read file, setting seek back to the start
    fn read<F>(&self, fun: F) -> Result<D, Error>
    where F: Fn(&File) -> Result<D, Error>
    {
        let mut file = self.open().context(ErrorKind::Database)?;
        let ret = fun(&file).context(ErrorKind::Database)?;
        file.seek(SeekFrom::Start(0)).context(ErrorKind::Database)?;
        Ok(ret)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::collections::HashMap;
    #[test]
    fn save() {
        pretty_env_logger::try_init();
        let db = SimpleDB::<HashMap<String, usize>>::new(PathBuf::from("/tmp/SOME")).unwrap();
        let mut data = HashMap::new();
        data.insert("Hello".to_string(), 45);
        data.insert("Byte".to_string(), 34);
        db.save(data.clone()).unwrap();
    }

    #[test]
    fn get() {
        pretty_env_logger::try_init();
        let db = SimpleDB::<HashMap<String, usize>>::new(PathBuf::from("/tmp/SOME")).unwrap();
        let mut data = HashMap::new();
        data.insert("Hello".to_string(), 45);
        data.insert("Byte".to_string(), 34);
        db.save(data.clone()).unwrap();
        info!("DATA: {:?}", db.get().unwrap());
    }
}

use std::{
	fmt::{self},
	mem::{size_of},
	fs::{self,DirEntry},
	io::{Error,ErrorKind,Read},
	sync::{Arc,Mutex},
	thread::{self,JoinHandle},
	cmp::Ordering,
};
use hex;
use structopt::StructOpt;
use byteorder::{LittleEndian,ReadBytesExt};
use log::{error};
use std::io::Cursor;

const MAGIC_NUMBER: u32 = 0xdeadbeef;
const ADDR_LEN: usize = 20;
const HASH_LEN: usize = 32;
const EXPECTED_NUM_ROWS_TO_FIND: usize = 256;

type Address = [u8; ADDR_LEN as usize];
type EthHash = [u8; HASH_LEN as usize];

/// header of the Address Table. 
#[derive(Debug)]
struct Header {
	magic:				u32,			// magic code identifying file format
	hash:				EthHash,	// unused
	num_addresses:		u32,			// number of files in the Address table
	num_appearrances:	u32,		// number of rows in Appearance table
}
impl Header {
	fn make_header(cursor: &mut Cursor<Vec<u8>>) -> Header {

		let mut hdr = Header {
			magic:				0,			
			hash:				[0; HASH_LEN],
			num_addresses:		0,
			num_appearrances:	0,
		};
		hdr.magic = cursor.read_u32::<LittleEndian>().unwrap();
		cursor.set_position(cursor.position() as u64 + HASH_LEN as u64);	// we aren't reading hash becasue it is 'unused'
		hdr.num_addresses = cursor.read_u32::<LittleEndian>().unwrap();
		hdr.num_appearrances= cursor.read_u32::<LittleEndian>().unwrap();
		hdr
	}
}

/// Address record.
#[derive(Clone,Debug)]
struct AddressRec {
	bytes:		Address,
	offset:		u32,
	cnt:		u32,
}
impl AddressRec {
	pub fn new() -> Self {
		AddressRec {
			bytes:		[0; ADDR_LEN],
			offset:		0,
			cnt:		0,
		}
	}
	fn make_address_rec(cursor: &mut Cursor<Vec<u8>>) -> AddressRec {

		let mut a = AddressRec::new();
		cursor.read(&mut a.bytes).unwrap();
		a.offset= cursor.read_u32::<LittleEndian>().unwrap();
		a.cnt= cursor.read_u32::<LittleEndian>().unwrap();
		a
	}
}
impl fmt::Display for AddressRec {
	fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
		write!(f,"{{addr: {}, offset: {}, cnt: {}}}",hex::encode(self.bytes),self.offset,self.cnt)
	}
}

/// Appearrance record. 
#[derive(Debug,Clone)]
struct AppearranceRec {
	blk:		u32,
	txid:		u32,
}
impl AppearranceRec {
	pub fn new() -> Self {
		AppearranceRec {
			blk:		0,
			txid:		0,
		}
	}
	fn make_appearrance_rec(cursor: &mut Cursor<Vec<u8>>) -> AppearranceRec {

		let mut a = AppearranceRec::new();
		a.blk= cursor.read_u32::<LittleEndian>().unwrap();
		a.txid= cursor.read_u32::<LittleEndian>().unwrap();
		a
	}
}
impl fmt::Display for AppearranceRec {
	fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
		write!(f,"{{blk: {}, txid: {}}}",self.blk,self.txid)
	}
}
struct ScanResult {
	index:		u32,
	appear:		Vec<AppearranceRec>,
	rerr:		Option<Error>,
}

#[derive(StructOpt)]
#[structopt(
	about = "Parallel File Scanner",
	author = "Author <user@host.domain.tld>",
	name = "filescan",
	version = "1.0"
)]

struct Opts {
	#[structopt(subcommand)]
	subcommands: Commands,
}
#[derive(StructOpt)]
enum Commands {
	/// Scan directory and its contents for pattern (String)
	#[structopt(name = "scan")]
	Scan {
		dir: String,
		addr: String,
	},
}
// Unfortunately we have to code our own conversion functions for creating the structs because:
//		We can't use transmute() , since the integers could be out of alignment inside the buffer
//		We can't use mmap() because it not a cross platform solution

fn launch_scan_on_file(file_entry: DirEntry, result: Arc<Mutex<ScanResult>>,srch_addr: Address) -> JoinHandle<()> {
	thread::spawn(move || {
		let fname = match file_entry.path().into_os_string().into_string() {
			Ok(name) => name,
			Err(_) => String::from(""),
		};
		let capacity = match file_entry.metadata() {
			Ok(md) => {
				if md.file_type().is_dir() {
					let mut r=result.lock().unwrap();
					let err_str = format!("File #{} {} is a directory",r.index,fname);
					error!("{}",err_str);
					let e = Error::new(ErrorKind::Other, err_str);
					r.rerr = Some(e);
					return;
				}
				md.len() as usize
			},
			Err(err)=> {
				let mut r=result.lock().unwrap();
				let err_str = format!("Can't read metadata for file #{} {}: {}",r.index,fname,err);
				error!("{}",err_str);
				let e = Error::new(ErrorKind::Other, err_str);
				r.rerr = Some(e);
				return;
			}
		};
		let mut file = match fs::File::open(&file_entry.path()) {
			Ok(f) => f,
			Err(err) => {
				let mut r=result.lock().unwrap();
				let err_str = format!("Failed to open file #{} {} : {}",r.index,fname,err);
				error!("{}",err_str);
				let e = Error::new(ErrorKind::Other, err_str);
				r.rerr = Some(e);
				return;
			},
		};
		let mut buffer = Vec::with_capacity(capacity + 1);

		match file.read_to_end(&mut buffer) {		// we read all, so later we can add search function on this buffer
			Ok(_) => (),
			Err(err) => {
				let mut r=result.lock().unwrap();
				let err_str = format!("Failed to read file #{} ({}) contents: {}",r.index,fname,err);
				error!("{}",err_str);
				let e = Error::new(ErrorKind::Other, err_str);
				r.rerr = Some(e);
				return;
			}
		}

		let buffer_len = buffer.len();
		if buffer_len < size_of::<Header>() {
			let mut r=result.lock().unwrap();
			let err_str = format!("File #{} {} is too small (size < size of Header struct, size={})",r.index,fname,buffer.len());
			let e = Error::new(ErrorKind::Other, err_str);
			r.rerr = Some(e);
			return;
		}

		let mut cursor = Cursor::new(buffer);
		let header = Header::make_header(&mut cursor);
		//println!("header: {:?}",header);

		if header.magic != MAGIC_NUMBER {
			let mut r=result.lock().unwrap();
			let err_str = format!("File #{} {} has bad magic number {}",r.index,fname,header.magic);
			error!("{}",err_str);
			let e = Error::new(ErrorKind::Other, err_str);
			r.rerr = Some(e);
			return;
		}

		let expected_file_len : u64 =	header.num_addresses as u64 * size_of::<AddressRec>() as u64 +
										header.num_appearrances as u64 * size_of::<AppearranceRec>() as u64 + 
										size_of::<Header>() as u64;
		if buffer_len != expected_file_len as usize {
			let mut r=result.lock().unwrap();
			let err_str = format!(
				"File #{} {} doesn't have proper length (bytes). \
				Must have {} = {} + {}*{} + {}*{} , but has {}",
				r.index,fname,
				expected_file_len,
				size_of::<Header>(),size_of::<AddressRec>(),header.num_addresses,
				size_of::<AppearranceRec>(),header.num_appearrances,
				buffer_len
			);
			error!("{}",err_str);
			let e = Error::new(ErrorKind::Other, err_str);
			r.rerr = Some(e);
			return;
		}
		
		let mut output : Vec<AppearranceRec> = Vec::with_capacity(EXPECTED_NUM_ROWS_TO_FIND);
		for _rec_no in 0..header.num_addresses {
			let addr_rec = AddressRec::make_address_rec(&mut cursor);
//			println!("Address Record = {}",addr_rec);
//			println!("{}",hex::encode(address_rec.bytes));
			let mut found = true;
			for i in 0..ADDR_LEN {
				if addr_rec.bytes[i]!=srch_addr[i] {
					found = false;
					break;
				}
			}
			if !found {
				continue;
			}
			let new_pos = 	size_of::<Header>() as u64 +
							header.num_addresses as u64 * size_of::<AddressRec>() as u64 +
							addr_rec.offset as u64 * size_of::<AppearranceRec>() as u64; 
			cursor.set_position(new_pos);
			for _ in 0..addr_rec.cnt {
				let appearrance = AppearranceRec::make_appearrance_rec(&mut cursor);
				output.push(appearrance);
			}
			break;
		}
		let mut r=result.lock().unwrap();
		r.appear= output;
		r.rerr = None;

	})
}

fn collect_results(dir: String,srch_addr: Address) -> Option<Vec<AppearranceRec>> {
	let dir_iter = match fs::read_dir(dir) {
		Ok(d) => d,
		Err(err) => {
			error!("Couldn't read directory: {}",err);
			return None;
		}
	};
	let mut thread_handles: Vec<JoinHandle<()>> = Vec::new();
	let mut thread_safe_results: Vec<Arc<Mutex<ScanResult>>> = Vec::new();
	let mut index: u32 = 0;
	for file_opt in dir_iter {
		match file_opt {
			Ok(file) => {
				let result = ScanResult {
					index: 	index,
					appear:	Vec::new(),
					rerr:	None,
				};
				let r = Arc::new(Mutex::new(result));
				thread_handles.push(launch_scan_on_file(file,r.clone(),srch_addr));
				thread_safe_results.push(r);
			},
			Err(err)=> {
				error!("Error accesing directory entry: {}",err);
			},
		}
		index += 1;
	}
	for th in thread_handles {
		th.join().unwrap()
	}
	let mut results: Vec<AppearranceRec> = Vec::with_capacity(thread_safe_results.len());
	for r in thread_safe_results {
		let thread_result = r.lock().unwrap();
		match thread_result.rerr {
			Some(_) => (),
			None => {
				for entry in &thread_result.appear {
					results.push((*entry).clone());
				}
			}
		}
	}
	if results.is_empty() { 	
		None
	} else {	
		Some(results)	
	}
}

fn result_vec_to_str(data: &Vec<AppearranceRec>) -> String {

	let mut output: String = String::with_capacity(2048);
	for d in data {
		if !output.is_empty() {
			output.push(',');
		}
		output.push_str(&format!("{}",d));
	}
	output
}

fn main() {

	env_logger::init();

	let Opts { subcommands } = Opts::from_args();

	match subcommands {
		Commands::Scan{dir,addr} => {
			let decoded = match hex::decode(addr) {
				Ok(a)=> {
					if a.len() != ADDR_LEN {
						println!("Address must be {} hex characters",ADDR_LEN);
						std::process::exit(1);
					}
					a
				},
				Err(err) => {
					println!("Cant decode address: {}",err);
					std::process::exit(1);
				},
			};
			let mut srch_addr: Address = [0; ADDR_LEN];
			for i in 0..ADDR_LEN {
				srch_addr[i] = decoded[i];
			}
			let mut results = match collect_results(dir,srch_addr.clone()) {
				Some(r) => r,
				None => {
					println!("Nothing found");
					std::process::exit(2);
				}
			};
//			println!("Unsorted: {}",result_vec_to_str(&results));
			results.sort_by(| r1, r2| { 
				if r1.blk>r2.blk {
					Ordering::Greater
				} else {
					if r1.blk < r2.blk {
						Ordering::Less
					} else {
						Ordering::Equal
					}

				}
			});
			println!("{}",result_vec_to_str(&results));
			std::process::exit(0);
		}
	}
}


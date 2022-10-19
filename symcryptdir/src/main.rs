use std::io::{BufRead, BufReader, Error, ErrorKind::Other, Read, Write};
use std::{env, fs::File};

const USAGE: &str = "usage: symcrypt <infile keyfile >outfile";

/// Processes and returns one program argument
pub fn progarg() -> String {
    let args: Vec<_> = env::args().collect();
    match args.len() {
        // no arguments passed
        1 => panic!("Missing argument!\n{USAGE}"),
        2 => args[1].to_owned(),
        _ => panic!("Too many arguments!\n{USAGE}"),
    }
}
/// reads stdin and a specified input file
/// writes xor-ed data to stdout
fn main() -> Result<(), Error> {
    let keyname = &progarg();
    let mut keyreader = BufReader::new(File::open(keyname)?);
    let mut lockin = std::io::stdin().lock();
    let mut lockout = std::io::stdout().lock();

    loop {
        let bufin = lockin.fill_buf()?;
        if bufin.is_empty() { break; };
        let length = bufin.len();
        let bufkey = keyreader.fill_buf()?;
        // eprintln!("{} {}",length,bufkey.len());
        if length != bufkey.len() {
            return Err(Error::new(
                Other,
                format!("symcrypt: {keyname} size mismatch"),
            ));
        }
        let mut bufout: Vec<u8> = Vec::with_capacity(length);
        for (byte, keybyte) in bufin.bytes().zip(bufkey.bytes()) {
            bufout.push(byte?^keybyte?);
        }
        lockin.consume(length);
        keyreader.consume(length);
        lockout.write_all(&bufout)?
    }
    lockout.flush()?;
    Ok(())
}

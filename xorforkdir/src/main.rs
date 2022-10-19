use std::io::{BufRead, BufWriter, Read, Write};
use std::{env, fs::File};

const USAGE: &str = "usage: xorfork <infile keyfile >outfile";

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

/// xorfork reads from stdin
/// and writes encrypted data to stdout and keyfile.
fn main() -> Result<(), std::io::Error> {
    let keyname = &progarg(); 
    let mut ranbytes = File::open("/dev/urandom")?.bytes(); 
    let mut keywriter = BufWriter::new(File::create(keyname)?);
    let mut lockin = std::io::stdin().lock();
    let mut bufout = Vec::new();
    let mut lockout = std::io::stdout().lock();
    let mut bufkey = Vec::new();

    loop {
        let bufin = lockin.fill_buf()?;
        let length = bufin.len();
        if bufin.is_empty() { break; }; 
        for inbyte in bufin.bytes() { 
            let keybyte = ranbytes.next().unwrap()?; 
            bufout.push(inbyte?^keybyte);
            bufkey.push(keybyte)
        }
        lockin.consume(length);
        keywriter.write_all(&bufkey)?;
        bufkey.clear();  
        lockout.write_all(&bufout)?;
        bufout.clear();
    }
    keywriter.flush()?;
    lockout.flush()?;
    Ok(())
}

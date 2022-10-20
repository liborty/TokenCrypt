use std::io::{BufRead, BufWriter, Read, Write};
use std::{env, fs::File};

const USAGE: &str = "usage: xorfork <infile outfile >keyfile";

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
    let outname = &progarg(); 
    let mut ranbytes = File::open("/dev/urandom")?.bytes(); 
    let mut outwriter = BufWriter::new(File::create(outname)?);
    let mut bufout = Vec::new();
    let mut bufkey = Vec::new();
    let mut lockin = std::io::stdin().lock();
    let mut lockout = std::io::stdout().lock();
    loop {
        let bufin = lockin.fill_buf()?; // read stdin
        if bufin.is_empty() { break; }; // input finished
        let length = bufin.len();
        for inbyte in bufin.bytes() { // iterate through input bytes
            let keybyte = ranbytes.next().unwrap()?; // get random byte
            bufout.push(inbyte?^keybyte); // XOR to output file buffer
            bufkey.push(keybyte) // random byte to stdout buffer
        }
        lockin.consume(length); // consume the exact input bytes
        outwriter.write_all(&bufout)?; // write xored data to output file
        bufout.clear(); // empty output file buffer for reuse 
        lockout.write_all(&bufkey)?; // write random key data to stdout
        bufkey.clear(); // empty stdout buffer for reuse
    }
    outwriter.flush()?; // flush any leftovers to file
    lockout.flush()?;   // flush any leftovers to stdout
    Ok(())
}

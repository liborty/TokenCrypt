use std::{io::{BufRead,Read,Write,Error}};

/// reads buffered binary from stdin using a small buffer,
/// writes hex characters to stdout
fn main() -> Result<(),Error> {
  let stdin = std::io::stdin();
  //let mut bufin = BufReader::with_capacity(BUFFIN,stdin);
  let stdout = std::io::stdout();
  //let mut bufout = BufWriter::with_capacity(BUFFOUT,stdout);  
  let mut lockin = stdin.lock();
  let mut lockout = stdout.lock();
  loop {
    let bufin = match lockin.fill_buf() {
        Ok(it) => it,
        Err(err) => return Err(err),
    };   
    if bufin.is_empty() { break; };
    for byte in bufin.bytes() {
      write!(lockout,"{:02x}", byte?)?;
    }
    let length = bufin.len();
    lockin.consume(length);    
  }
  lockout.flush()?;
  Ok(())
}

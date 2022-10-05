use std::io::{BufRead, Error, ErrorKind::Other, Read, Write};

const ZERO: u8 = 48;
const LF: u8 = 253;
const SPACE: u8 = 254;
const NONHEX: u8 = 255;

/// returns numerical values (0-15) of hexadecimal characters 0-9,a-f,A-F
/// LF and SPACE are encoded as 253 and 254, otherwise returns NONHEX (255),
/// indicating non-hexadecimal data
fn ishex(n: u8) -> u8 {
  match n {
    10 => return LF,    // LF, recode as 253
    32 => return SPACE, // SPACE, recode as 254
    n if n < ZERO => return NONHEX, // reject all others below '0' (ascii 48)
    n if n < 58 => return n - ZERO,// convert digits to numbers 0-9
    n if (n > 96) && (n < 103) => return n - 87, // a-f, convert to 10-15
    n if (n > 64) && (n < 71) =>  return n - 55, // A-F, also convert to 10-15
    _=> return NONHEX // reject all others
  }
}

/// This utility reads text from stdin.
/// It fails on the first non-hexadecimal character.
/// Otherwise all verified hexadecimal data is packed
/// and written to stdout as binary, two hex chars per byte.
fn main() -> Result<(), Error> { 
    let mut lockin = std::io::stdin().lock();
    let mut lockout = std::io::stdout().lock();
    let mut hex:u8 = 0;
    loop {
        let bufin = match lockin.fill_buf() {
            Ok(it) => it,
            Err(err) => return Err(err),
        };
        if bufin.is_empty() {
            break;
        };
        let length = bufin.len();
        if (length % 2) == 1 {
            // this may arise only for the last chunk
            return Err(Error::new(Other, "hexcheck failed: input is of odd length"));
        }
        let mut hexesbuf:Vec<u8> = Vec::with_capacity(length/2);
        let mut firsthex = true;
        for byte in bufin.bytes() {
            let uc = ishex(byte?); // get hex values
            match uc {
                NONHEX =>
                // non hex, failure return
                {
                    return Err(Error::new(Other, "hexcheck: invalid input char"))
                }
                LF | SPACE => continue, // filter out all LF and SPACE characters
                _ => {
                    if firsthex {
                        // this is first hex char, shift it left
                        hex = uc << 4;
                        firsthex = false;
                    } else {
                        firsthex = true;
                        hexesbuf.push(hex|uc);  // bitwise or, packs two hex chars into a single byte                       
                    } 
                    }
                }
            }
        lockin.consume(length);
        lockout.write_all(&hexesbuf)? 
    }
    Ok(())
}
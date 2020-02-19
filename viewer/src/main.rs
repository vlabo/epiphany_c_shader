use std::io::prelude::*;
use std::fs::File;
use std::net::TcpStream;
// use std::{thread, time};
use common::*;
use lazy_static::lazy_static;
use std::sync::Mutex;
use std::env;
use std::io::{Result, Error, ErrorKind};

lazy_static! {
    static ref CONFIG : Mutex<Config> = Mutex::new(Config{width: 800, height: 800});
}

fn main() {
    if let Ok(mut stream) = TcpStream::connect("192.168.0.106:34254") {
    //if let Ok(mut stream) = TcpStream::connect("127.0.0.1:34254") {
        println!("Connected to server");
        //let sleep_time = time::Duration::from_millis(1000);
        if let Ok(_) = send_config(&mut stream) {
            if let Err(e) = send_code(&mut stream) {
                panic!("{}", e);
            }

            request_frame(&mut stream);
        } else {
            println!("Sending config failed");
        }

    } else {
        println!("Failed to connect to server");
    }
}

fn send_config(stream: &mut TcpStream) -> std::io::Result<()> {
    let command = VIEWER_CONFIG; 
    stream.write(&[command])?;
    let config = CONFIG.lock().unwrap().to_str();
    stream.write(config.as_bytes())?;
    Ok(())
}

fn request_frame(stream: &mut TcpStream) {
    let command = VIEWER_FRAME_REQUEST; 
    stream.write(&[command]).expect("Failed to request a frame");
    let mut response = [0; 1];
    if let Ok(()) = stream.read_exact(&mut response) {
        if SERVER_FRAME == response[0] {
            let config = CONFIG.lock().unwrap();
            let bytes_count = config.get_bytes_count();
            let mut byte_array = vec![0; bytes_count];
            if let Err(e) = stream.read_exact(&mut byte_array) {
                panic!("Error while reading frame. {}", e);
            }
            image::save_buffer("image.png", &byte_array, config.width, config.height, image::ColorType::Rgba8).unwrap();
            println!("Received frame.");
        } 
    }
}

fn send_code(stream: &mut TcpStream) -> Result<()> {
    let mut args = env::args();
    if args.len() <= 1 {
        panic!("No input file");
    }

    let _ = args.next().unwrap();
    let file_name = args.next().unwrap();
    println!("File name: {}", &file_name);
    let mut file = File::open(file_name)?;
    let mut code = String::new();
    file.read_to_string(&mut code).unwrap();

    let command = VIEWER_CODE; 
    stream.write(&[command])?;
    let size_str = format!("{:08}", code.len());
    stream.write(size_str.as_bytes())?;
    stream.write(code.as_bytes())?;
    let mut response = [0; 1];
    stream.read_exact(&mut response)?;

    match response[0] {
        SERVER_COMPILE_COMPLETE => Ok(()),
        SERVER_COMPILE_ERROR => {
            let mut error_size_array = [0; 8];
            stream.read_exact(&mut error_size_array)?; 
            let error_size = std::str::from_utf8(&error_size_array).unwrap().parse::<usize>().unwrap(); 
            let mut error_array = vec![0; error_size];
            stream.read_exact(&mut error_array)?;
            let error = std::str::from_utf8(&error_array).unwrap();
            Err(Error::new(ErrorKind::Other, format!("Compilation error: \n {}", error)))
        },
        _ => Err(Error::new(ErrorKind::Other, format!("Unknown server response: {}", response[0]))) 
    }
}

use std::io::prelude::*;
use std::fs::File;
use std::net::TcpStream;
use std::{thread, time};
use common::*;
use num_traits::{ToPrimitive, FromPrimitive};
use lazy_static::lazy_static;
use std::sync::Mutex;
use std::env;

lazy_static! {
    static ref CONFIG : Mutex<Config> = Mutex::new(Config{width: 1000, height: 1000});
}

fn main() {
    if let Ok(mut stream) = TcpStream::connect("127.0.0.1:34254") {
        println!("Connected to server");
        let sleep_time = time::Duration::from_millis(1000);
        if let Ok(_) = send_config(&mut stream) {
            send_code(&mut stream);    
            loop {
               request_frame(&mut stream);
               thread::sleep(sleep_time);
            }
        } else {
            println!("Sending config failed");
        }

    } else {
        println!("Failed to connect to server");
    }
}

fn send_config(stream: &mut TcpStream) -> std::io::Result<()> {
    let command = ToPrimitive::to_u8(&ViewerCommand::Config).unwrap();
    stream.write(&[command])?;
    let config_bytes = CONFIG.lock().unwrap().serialize();
    stream.write(&config_bytes)?;
    Ok(())
}

fn request_frame(stream: &mut TcpStream) {
    let command = ToPrimitive::to_u8(&ViewerCommand::FrameRequest).unwrap();
    stream.write(&[command]).expect("Failed to request a frame");
    let mut response = [0; 1];
    if let Ok(()) = stream.read_exact(&mut response) {
        if let Some(ServerCommand::Frame) = ServerCommand::from_u8(response[0]) {
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

fn send_code(stream: &mut TcpStream) {
    let mut args = env::args();
    if args.len() <= 1 {
        panic!("No input file");
    }

    let _ = args.next().unwrap();
    let file_name = args.next().unwrap();
    println!("File name: {}", &file_name);
    let mut file = File::open(file_name).expect("Source file not found");
    let mut code = String::new();
    file.read_to_string(&mut code).unwrap();

    let command = ToPrimitive::to_u8(&ViewerCommand::Code).unwrap(); 
    stream.write(&[command]).expect("Failed to send code command");
    let size = code.len();
    stream.write(&bincode::serialize(&size).unwrap()).expect("Failed to send code size.");
    stream.write(code.as_bytes()).expect("Failed to send code ");
}

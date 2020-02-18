mod epiphany;
use std::fs::File;
use std::io::prelude::*;
use std::net::{TcpListener, TcpStream};
use std::{thread, time};
use common::*;
use num_traits::{FromPrimitive, ToPrimitive};
use std::mem;
use epiphany::Epiphany;

fn handle_client(mut stream: TcpStream) {
    let mut epiphany = Epiphany::new(0, 0);
    loop {
        let mut data = [0; 1];
        if let Ok(()) = stream.read_exact(&mut data) {
            process_command(FromPrimitive::from_u8(data[0]).unwrap(), &mut stream, &mut epiphany);
        } else {
            return;
        }
    } 
}

fn process_command(command: ViewerCommand, stream: &mut TcpStream, epiphany: &mut Epiphany) {
    match command {
        ViewerCommand::FrameRequest => {
            if let Err(e) = send_frame(stream, epiphany) {
                println!("Failed to send frame. {}", e);
            }
        },
        ViewerCommand::Config => {
            let mut byte_array = [0; mem::size_of::<Config>()];
            if let Ok(()) = stream.read_exact(&mut byte_array) {
                let config : Config = bincode::deserialize(&byte_array).unwrap();
                println!("Config received {}x{}", config.width, config.height);
                epiphany.configure(config.width, config.height);
            }

        },
        ViewerCommand::Code => {
            let mut byte_array = [0; mem::size_of::<usize>()];
            if let Ok(()) = stream.read_exact(&mut byte_array) {
                let size : usize = bincode::deserialize(&byte_array).unwrap();
                let mut code_vec = vec![0; size];
                if let Err(e) = stream.read_exact(&mut code_vec) {
                    panic!("Failed to receive code. {}", e);
                }
                let code = std::str::from_utf8(&code_vec).unwrap();
                compile_code(code);
            }
        }
        _ => {
            println!("Command not supported!");
        }
    }
}

fn send_frame(stream: &mut TcpStream, epiphany: &mut Epiphany) -> std::io::Result<()> {
    epiphany.update_frame();
    let command = ServerCommand::Frame.to_u8().unwrap();
    stream.write(&[command])?;
    stream.write(epiphany.get_data())?;
    Ok(())
}

fn compile_code(code: &str) {
    let mut file = File::create("epiphany.c").expect("Failed to create file");
    file.write_all(code.as_bytes()).expect("Failed to write to file");
}

fn main() {
    let sleep_time = time::Duration::from_millis(100);

    if let Ok(listener) = TcpListener::bind("127.0.0.1:34254") {
        loop {
            
            for stream in listener.incoming() {
                handle_client(stream.unwrap());
            }
            thread::sleep(sleep_time);
        }
    } else {
        println!("Failed to open the tcp server.");
    }
}

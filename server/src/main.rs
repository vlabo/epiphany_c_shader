mod epiphany;
use std::fs::File;
use std::io::prelude::*;
use std::net::{TcpListener, TcpStream};
use std::{thread, time};
use epiphany::Epiphany;
use common::*;
use std::process::Command;

fn handle_client(mut stream: TcpStream) {
    let mut epiphany = Epiphany::new(0, 0);
    loop {
        let mut data = [0; 1];
        if let Ok(()) = stream.read_exact(&mut data) {
            process_command(data[0], &mut stream, &mut epiphany);
        } else {
            return;
        }
    } 
}

fn process_command(command: u8, stream: &mut TcpStream, epiphany: &mut Epiphany) {
    match command {
        VIEWER_FRAME_REQUEST => {
            if let Err(e) = send_frame(stream, epiphany) {
                println!("Failed to send frame. {}", e);
            }
        },
        VIEWER_CONFIG => {
            let mut byte_array = [0; 16];
            if let Ok(()) = stream.read_exact(&mut byte_array) {
                let config = Config::from_bytes(byte_array);
                println!("Config received {}x{}", config.width, config.height);
                epiphany.configure(config.width, config.height);
            }

        },
        VIEWER_CODE => {
            let mut byte_array = [0; 8];
            if let Ok(()) = stream.read_exact(&mut byte_array) {
                let size_str = std::str::from_utf8(&byte_array).unwrap();
                let size = size_str.parse::<usize>().unwrap();
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
    let command = SERVER_FRAME; 
    stream.write(&[command])?;
    stream.write(epiphany.get_data())?;
    Ok(())
}

fn compile_code(code: &str) {
	let mut template = include_str!("../e_template.c");
	let full_code = template.replace("<!--shader-->", code);

    let mut file = File::create("epiphany.c").expect("Failed to create file");
    file.write_all(full_code.as_bytes()).expect("Failed to write to file");
	let command = "e-gcc -T /opt/adapteva/esdk/bsps/current/internal.ldf -I ./c epiphany.c -o e_main.elf -le-lib -lm";
    let output = Command::new("sh")
            .arg("-c")
            .arg(&command)
            .output()
            .expect("failed to execute process"); 

	if !output.status.success() { 
		println!("{:?}", output);
	}
}

fn main() {
    let sleep_time = time::Duration::from_millis(100);

    //if let Ok(listener) = TcpListener::bind("127.0.0.1:34254") {
    if let Ok(listener) = TcpListener::bind("192.168.0.106:34254") {
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

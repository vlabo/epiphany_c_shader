#[macro_use]
extern crate num_derive;

use serde::{Serialize, Deserialize};


#[derive(FromPrimitive, ToPrimitive)]
pub enum ViewerCommand {
    Code = 1,
    Config,
    Stop,
    FrameRequest,
}

#[derive(FromPrimitive, ToPrimitive)]
pub enum ServerCommand {
    Frame = 1,
    Error,
}

#[derive(Serialize, Deserialize)]
pub struct Config {
    pub width: u32,
    pub height: u32,
}

impl Config {
    pub fn get_bytes_count(&self) -> usize {
        return (self.width * self.height * 4) as usize;
    }

    pub fn serialize(&self) -> Vec<u8> {
        bincode::serialize(self).unwrap()
    }
}
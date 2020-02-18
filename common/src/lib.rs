pub const VIEWER_CODE : u8 = 1;
pub const VIEWER_CONFIG : u8 = 2;
pub const VIEWER_FRAME_REQUEST : u8 = 3;

pub const SERVER_FRAME : u8 = 1;
pub const SERVER_COMPILE_ERROR : u8 = 2;
pub const SERVER_COMPILE_COMPLETE : u8 = 3;

pub struct Config {
    pub width: u32,
    pub height: u32,
}

impl Config {
    pub fn get_bytes_count(&self) -> usize {
        return (self.width * self.height * 4) as usize;
    }

    pub fn from_bytes(bytes: [u8; 16]) -> Self {
        let width_str = std::str::from_utf8(&bytes[0..8]).unwrap();
        let height_str = std::str::from_utf8(&bytes[8..]).unwrap();

        Config {
            width: width_str.parse::<u32>().unwrap(),
            height: height_str.parse::<u32>().unwrap(),
        }
    } 

    pub fn to_str(&self) -> String {
        format!("{:08}{:08}", self.width, self.height)
    }
}
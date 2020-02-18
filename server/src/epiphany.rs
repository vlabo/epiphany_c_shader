extern "C" {
    fn epiphany_init();
    fn epiphany_destroy();
    fn epiphany_alloc(width: u32, height: u32) -> *mut u8;
    fn epiphany_free(image_data: *mut u8);
    fn get_frame(image_data: *mut u8);
}

pub struct Epiphany {
    image_data: *mut u8,
    width: u32,
    height: u32,
}

impl Epiphany {
    pub fn new(width: u32, height: u32) -> Self {
        unsafe { epiphany_init() };
        let image_data = if width > 0 && height > 0 {
            unsafe { epiphany_alloc(width, height) }
        } else {
            std::ptr::null_mut() 
        };

        Epiphany {image_data, width, height}
    }

    pub fn configure(&mut self, width: u32, height: u32) {
        self.width = width;
        self.height = height;

        if self.image_data != std::ptr::null_mut() {
            unsafe { epiphany_free(self.image_data); }
            self.image_data = std::ptr::null_mut();
        }

        if width > 0 && height > 0 {
            self.image_data = unsafe { epiphany_alloc(width, height) };
        }
    }

    pub fn update_frame(&self) {
        unsafe {
            get_frame(self.image_data);
        }
    }

    pub fn get_data(&self) -> &[u8] {
        unsafe { 
            std::slice::from_raw_parts( self.image_data, self.get_bytes_count()) 
        }
    }

    pub fn get_bytes_count(&self) -> usize {
        (self.width * self.height * 4) as usize
    }
}

impl Drop for Epiphany {
    fn drop(&mut self) {
        unsafe {
            epiphany_free(self.image_data);
            epiphany_destroy();
        }
    }
}

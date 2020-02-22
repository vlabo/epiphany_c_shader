# Epiphany C Shaders

This is a small test server client application with witch you can send small **c** based shader function to the [parallella board](https://www.parallella.org/). The parallella will compute the function and return a image;

> This is a test project all the IP addresses are hardcoded. If you want to run it you may need to change then in the code.  

## Run

### Parallella

1. Install [rust](https://www.rust-lang.org/tools/install)
2. clone this repository.
3. Fix the Ip address in the `server/src/main.rs`.
4. `cd server`
5. `cargo run --release`

> The epiphany sdk should be installed. It goes whit the original parallella image.

### Client

 1. Clone the repository.
 2. Fix the Ip address in `viewer/src/main.rs`.
 3. `cd viewer`
 4. `cargo build --release`
 5. `cargo run --release shaders/gradient.c`

> It should work on any platform that **rust** it supported.

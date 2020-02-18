fn main() {
    cc::Build::new()
        .file("c/lib.c")
        .flag("-std=c99")
        .flag("-O3")
        .compile("lib");
}
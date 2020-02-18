fn main() {
  cc::Build::new()
      .file("c/lib.c")
      .flag("-std=c99")
      .flag("-O3")
      .flag("-L /opt/adapteva/esdk/tools/host/lib")
      .include("/opt/adapteva/esdk/tools/host/include")
      .include("./c")
      .compile("lib");


	println!("cargo:rustc-flags=-L /opt/adapteva/esdk/tools/host/lib");
	println!("cargo:rustc-flags=-le-hal");
	println!("cargo:rustc-flags=-le-loader");
}

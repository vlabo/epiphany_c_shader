fn main() {
  cc::Build::new()
      .file("c/lib.c")
      .flag("-std=c99")
      .flag("-O3")
      .flag("-L /opt/adapteva/esdk/tools/host/lib")
      .flag("-I /opt/adapteva/esdk/tools/host/include")
      .flag("-I c")
      .flag("-le-hal").flag("-le-loader")
      .compile("lib");
}
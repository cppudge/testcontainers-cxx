use std::env;

fn main() {
    let std = &env::var("RUST_CXX_STD").unwrap_or("c++17".to_string());
    let pic_opt = env::var("RUST_PIC").ok().map(|v| v == "1");

    let mut build = cxx_build::bridge("src/lib.rs");

    build.cpp(true).std(std);

    if let Some(pic) = pic_opt {
        build.pic(pic);
    }

    build.compile("tc_bridge");
}

use std::net::{TcpListener, TcpStream};
use std::fs::File;
use std::io::BufWriter;
use std::io::prelude::*;

extern crate zerver;
use zerver::ThreadPool;

const DEFAULT_SERVER_PORT: i16 = 5051;
const THREADS: usize = 2000;

fn send_headers(stream: &mut TcpStream, len: u64) {
    //println!("sending header");
    let mut writer = BufWriter::new(stream);
    let header = format!("HTTP/1.0 200 OK\r\n\
                  Content-Type: text/html\r\n\
                  content-length: {}\r\n\
                  \r\n", len);
    writer.write_all(header.as_bytes()).unwrap();
}

//fn send_file_contents() {
//    println!("Sending file contents");
//    let mut writer = BufWriter::new(stream);
//    writer.write_all(resp404.as_bytes()).unwrap();
//}

fn handle_http_404(stream: &mut TcpStream) {
    let resp404 = "HTTP/1.0 404 Not Found\r\n\
        Content-Type: text/html\r\n\
        \r\n\
        <html>
        <head>
        <title>Zerver: Not Found</title>
        </head>
        <body>
        <h1>Not Found (404)</h1>
        <p>Client request for an object not found on the server.</p>
        </body>
        </html>";

    //send_headers(stream, resp404.len() as u64);

    let mut writer = BufWriter::new(stream);
    writer.write_all(resp404.as_bytes()).unwrap();
}

fn handle_get_method(stream: &mut TcpStream, path: &str) {
    let mut p = String::from("public/");
    // switch on last char to see if client is asking for dir
    if path.chars().last().unwrap() == '/' {
        p.push_str(path);
        p.push_str("index.html")
    } else {
        p.push_str(path);
    }

    let f = File::open(p);
    if f.is_err() {
        handle_http_404(stream);
        return;
    }
    let mut fi = f.unwrap();

    let meta = fi.metadata().unwrap();

    let mut buffer = Vec::new();
    // read the whole file
    fi.read_to_end(&mut buffer).unwrap();

    send_headers(stream, meta.len());

    //println!("{:?}", f);
    //println!("Sending file contents");
    let mut writer = BufWriter::new(stream);
    writer.write_all(&buffer).unwrap();
}

fn handle_http_method(stream: &mut TcpStream, ln: String) {
    let split = ln.split(" ");
    let vec: Vec<&str> = split.collect();

    let method = vec[0];
    let path = vec[1];
    // protocol NYI
    let _protocol = vec[2];
    if method != "GET" {
        eprintln!("Unsupported HTTP method");
    }
    handle_get_method(stream, path);
}

fn handle_client(stream: &mut TcpStream) {
    //println!("Got request!");

    // why does this have to be mutable?
    // read the whole file -- fails (hangs) due to not terminating somehow?
    //let mut buffer = Vec::new();
    //stream.read_to_end(&mut buffer).expect("Failed to read TcpStream to buf");
    //or we can do fixed size
    let mut buffer = [0; 1024];
    stream.read(&mut buffer).unwrap();
    for line in buffer.lines() {
        //println!("{:?}", line); //these are all surrounded in Ok()
        let ln = line.unwrap();
        println!("{}", ln);
        /*
         * let get = b"GET / HTTP/1.1\r\n";
         * if buffer.starts_with(get) {
         */
        handle_http_method(stream, ln);
        break; // only using first line
    }
    //println!("{:?}", buffer);
}

fn main() -> std::io::Result<()> {


    let host = format!("127.0.0.1:{}", DEFAULT_SERVER_PORT);
    let listener = TcpListener::bind(host).unwrap();

    let pool = ThreadPool::new(THREADS);

    // accept connections and process them serially
    for stream in listener.incoming() {
        let mut stream = stream.unwrap();
        pool.execute(move || {
            handle_client(&mut stream);
        });
    }
    Ok(())
}

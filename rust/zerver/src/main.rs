use std::net::{TcpListener, TcpStream};
use std::fs::File;
use std::io::BufWriter;
use std::io::prelude::*;

const DEFAULT_SERVER_PORT: i16 = 5051;

fn send_headers(stream: &mut TcpStream, path: String) {
    println!("sending header");
    let mut writer = BufWriter::new(stream);
    let header = "HTTP/1.0 200 OK\r\n\
                  Content-Type: text/html\r\n\
                  content-length: 5000\r\n\
                  \r\n";
    writer.write_all(header.as_bytes()).unwrap();
}

//fn send_file_contents() {
//    println!("Sending file contents");
//    let mut writer = BufWriter::new(stream);
//    writer.write_all(resp404.as_bytes()).unwrap();
//}

fn handle_http_404(stream: &mut TcpStream) {
    let resp404 = "HTTP/1.0 404 Not Found\r
        Content-type: text/html\r
        \r
        <html>
        <head>
        <title>Zerver: Not Found</title>
        </head>
        <body>
        <h1>Not Found (404)</h1>
        <p>Client request for an object that was not found on this server.</p>
        </body>
        </html>";

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
    }

    let mut buffer = Vec::new();
    // read the whole file
    f.unwrap().read_to_end(&mut buffer);

    //println!("{:?}", f);
    println!("Sending file contents");
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
    println!("Got request!");

    // why does this have to be mutable?
    // read the whole file -- fails (hangs) due to not terminating somehow?
    //let mut buffer = Vec::new();
    //stream.read_to_end(&mut buffer).expect("Failed to read TcpStream to buf");
    //or we can do fixed size
    let mut buffer = [0; 1024];
    stream.read(&mut buffer);
    for line in buffer.lines() {
        println!("{:?}", line); //these are all surrounded in Ok()
        let ln = line.unwrap();
        println!("{}", ln);
        handle_http_method(stream, ln);
        break; // only using first line
    }
    //println!("{:?}", buffer);
}

fn main() -> std::io::Result<()> {

    let host = format!("127.0.0.1:{}", DEFAULT_SERVER_PORT);
    let listener = TcpListener::bind(host)?;

    // accept connections and process them serially
    for stream in listener.incoming() {
        handle_client(&mut stream?);
    }
    Ok(())
}

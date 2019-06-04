//extern crate nix;
use std::thread;
use std::os::unix::net::{UnixStream, UnixListener};

const DEFAULT_SERVER_PORT: i16 = 5051;

fn handle_client(stream: UnixStream) {
    println!("Got request!")
}

fn setup_listen_socket(port: i16) {
    //sock;
    //struct sockaddr_in server_addr;

    let listener = UnixListener::bind("localhost:5051").unwrap();

    // accept connections and process them, spawning a new thread for each one
    for stream in listener.incoming() {
        match stream {
            Ok(stream) => {
                /* connection succeeded */
                thread::spawn(|| handle_client(stream));
            }
            Err(err) => {
                /* connection failed */
                break;
            }
        }
    }

    // create TCP socket
    // socket(AF_INTET, socket_type, protocol) in C
    //sock = nix::sys::socket::socket<>();
}

fn main() {
    let server_port = DEFAULT_SERVER_PORT;
    let server_socket = setup_listen_socket(server_port);
    println!("Running zerverd on port 5051");
}

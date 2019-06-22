## Setup
```bash
$ brew install rustup
$ rustup-init
$ rustc --version
```

## Usage
```bash
$ rustc main.rs
```
```bash
$ cargo run
```

## Reading
https://doc.rust-lang.org/1.24.1/book/second-edition/ch20-01-single-threaded.html

## Testing

``` bash
wrk2 -t4 -c100 -d30s -R1000 http://127.0.0.1:5051
```

```
Running 30s test @ http://127.0.0.1:5051
  4 threads and 100 connections
  Thread calibration: mean lat.: 1.502ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 1.396ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 1.379ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 1.385ms, rate sampling interval: 10ms
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     1.41ms  727.59us   9.86ms   77.27%
    Req/Sec    89.43    138.21   700.00     86.94%
  16433 requests in 30.13s, 6.25MB read
  Socket errors: connect 0, read 0, write 0, timeout 600
Requests/sec:    545.32
Transfer/sec:    212.48KB
```

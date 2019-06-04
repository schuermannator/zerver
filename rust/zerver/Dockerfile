FROM rust:1.31

WORKDIR /usr/src/zerverd
COPY . .

RUN cargo install --path .

CMD ["zerver"]

FROM ubuntu:latest
RUN apt-get update && apt-get install -y make gcc
COPY . /app
WORKDIR /app
RUN make
CMD ./main
#ENTRYPOINT ["./main"]

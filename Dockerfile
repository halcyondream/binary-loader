FROM alpine:latest
COPY . /build
WORKDIR /build
RUN apk update && apk add binutils-dev g++ make
RUN cd /build && make loader
# TODO: Accept a CLI arg instead.
ENTRYPOINT ["/build/a.out", "/bin/ls"]  
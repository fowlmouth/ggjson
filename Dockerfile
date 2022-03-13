FROM alpine:3.15

RUN apk add --no-cache build-base git

WORKDIR /build

ADD Makefile ./
ADD include/ include/
ADD src/ src/

RUN make -j4

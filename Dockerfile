FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/app

COPY dining_philosophers.c .

RUN gcc -pthread -o dining_philosophers dining_philosophers.c

CMD [ "./dining_philosophers", "10" ]
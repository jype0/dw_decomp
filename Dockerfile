FROM ubuntu:noble

COPY requirements.txt requirements.txt

RUN apt-get update && \
    apt-get install -y \
    binutils-mipsel-linux-gnu \
    gcc-mipsel-linux-gnu \
    make \
    python3 \
    python3-venv \
    && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

USER ubuntu
WORKDIR /dw
COPY requirements.txt requirements.txt
RUN chown ubuntu:ubuntu /dw && \
    mkdir -p /dw/.venv /dw/build && \
    python3 -m venv .venv && \
    . .venv/bin/activate && \
    pip3 install -r requirements.txt

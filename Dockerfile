FROM ubuntu:noble

RUN apt-get update && \
    apt-get install -y \
    binutils-mipsel-linux-gnu \
    cpp \
    gcc-mipsel-linux-gnu \
    git \
    make \
    python3 \
    python3-venv \
    unzip \
    wget \
    clang \
    clangd \
    && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

RUN mkdir /dw && \
    chown ubuntu:ubuntu /dw

USER ubuntu
WORKDIR /dw

COPY requirements.txt requirements.txt

RUN mkdir -p /dw/.venv && \
    git config --global --add safe.directory /dw && \
    python3 -m venv .venv

ENV PATH="/dw/.venv/bin:$PATH"

RUN pip3 install -r requirements.txt

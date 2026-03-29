FROM ubuntu:noble

RUN apt-get update && \
    apt-get install -y \
    binutils-mipsel-linux-gnu \
    gcc-mipsel-linux-gnu \
    git \
    make \
    python3 \
    python3-venv \
    unzip \
    wget \
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
    python3 -m venv .venv && \
    . .venv/bin/activate && \
    pip3 install -r requirements.txt && \
    echo '. /dw/.venv/bin/activate' >> "$HOME/.bash_aliases"

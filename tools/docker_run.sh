#!/bin/sh

docker run -it --rm -v "$PWD":/dw -v dw_venv:/dw/.venv dw-build "$@"

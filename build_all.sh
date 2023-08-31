#!/bin/bash
set -eo pipefail

find . -name "build.sh" -maxdepth 2 -print -exec {} \;

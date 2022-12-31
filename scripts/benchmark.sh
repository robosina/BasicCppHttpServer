#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

#check active_server.txt exists
if [ ! -f active_server.txt ]; then
    echo -e "${RED}active_server.txt does not exist${NC}"
    exit 1
fi

# read active server address from file
SERVER_ADDRESS=$(cat active_server.txt)

# echo server address
echo -e "${GREEN}Server address: ${SERVER_ADDRESS}${NC}"

# run benchmark
ab -n 300000 -c 10000 "${SERVER_ADDRESS}"
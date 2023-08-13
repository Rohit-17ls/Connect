#!/bin/sh

echo $(gcc -o connect .src/connect.c && gcc -o client .src/client.c -pthread && gcc -o server .src/server.c -pthread);

#!/bin/sh
echo $(gcc -o ../connect .src/connect.c && gcc -o client .src/client.c -pthread && gcc -o server .src/server.c -pthread);
echo "Have a good time using Connect !!\nIn case you face any issue, start an issue on github at https://github.com/Sly-17/Connect/issues";

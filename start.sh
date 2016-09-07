#!  /bin/sh


echo "itcast" | sudo /usr/local/nginx/sbin/nginx

spawn-fcgi -a 127.0.0.1 -p 8081 -f ./demo

spawn-fcgi -a 127.0.0.1 -p 8043 -f ./upload

redis-server

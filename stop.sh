#!  /bin/sh

echo "itcast" | sudo /usr/local/nginx/sbin/nginx -s stop;  

PID=`ps -aux | grep demo | awk {'print $2'}`;

kill -9 $PID;

PID2=`ps -aux | grep upload | awk {'print $2'}`;

kill -9 $PID2;

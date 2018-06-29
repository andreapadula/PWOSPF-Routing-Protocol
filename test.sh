#!/bin/bash

command=$1
max=100
rm -f temp

if [ $# -eq 0 ]
then
 echo "ERROR YOU MUST INPUT A VHOST"
 exit
fi


if [ $command == 'vhost1' ] || [ $command == 'all' ]
then
    echo "****PINGING VHOST 1 eth0"
    ping -c 10 172.29.4.178 > temp
    VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
    score=$(echo "scale=1; $max - $VALUE " | bc -l)
    echo " RESULT for vhost1 eht0 $score"
    echo
    rm -f temp


    echo "****PINGING VHOST 1 eth1"
    ping -c 10 172.29.4.180 > temp
    VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
    score=$(echo "scale=1; $max - $VALUE " | bc -l)
    echo " RESULT for vhost1 eht1 $score"
    echo
    rm -f temp


    echo "****PINGING VHOST 1 eth2"
    ping -c 10 172.29.4.184 > temp
    VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
    score=$(echo "scale=1; $max - $VALUE " | bc -l)
    echo " RESULT for vhost1 eht2 $score"
    echo
    rm -f temp
fi



if [ $command == 'vhost2' ] || [ $command == 'all' ]
then
    echo "****PINGING VHOST 2 eth0"
    ping -c 10 172.29.4.181 > temp
    VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
    score=$(echo "scale=1; $max - $VALUE " | bc -l)
    echo " RESULT for vhost1 eht0 $score"
    echo
    rm -f temp


    echo "****PINGING VHOST 2 eth1"
    ping -c 10 172.29.4.182 > temp
    VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
    score=$(echo "scale=1; $max - $VALUE " | bc -l)
    echo " RESULT for vhost1 eht1 $score"
    echo
    rm -f temp


    echo "****PINGING VHOST 2 eth2"
    ping -c 10 172.29.4.189 > temp
    VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
    score=$(echo "scale=1; $max - $VALUE " | bc -l)
    echo " RESULT for vhost1 eht2 $score"
    echo
    rm -f temp
fi



if [ $command == 'vhost3' ] || [ $command == 'all' ]
then
    echo "****PINGING VHOST 3 eth0"
    ping -c 10 172.29.4.185 > temp
    VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
    score=$(echo "scale=1; $max - $VALUE " | bc -l)
    echo " RESULT for vhost1 eht0 $score"
    echo
    rm -f temp


    echo "****PINGING VHOST 3 eth1"
    ping -c 10 172.29.4.186 > temp
    VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
    score=$(echo "scale=1; $max - $VALUE " | bc -l)
    echo " RESULT for vhost1 eht1 $score"
    echo
    rm -f temp


    echo "****PINGING VHOST 3 eth2"
    ping -c 10 172.29.4.190 > temp
    VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
    score=$(echo "scale=1; $max - $VALUE " | bc -l)
    echo " RESULT for vhost1 eht2 $score"
    echo
    rm -f temp
fi



if [ $command == 's1' ] || [ $command == 'all' ]
then
    echo "****PINGING s1 "
    ping -c 10 172.29.4.183 > temp
    VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
    score=$(echo "scale=1; $max - $VALUE " | bc -l)
    echo " RESULT for s1 $score"
    echo
    rm -f temp
fi


if [ $command == 's2' ] || [ $command == 'all' ]
then
    echo "****PINGING s2 "
    ping -c 10 172.29.4.187 > temp
    VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
    score=$(echo "scale=1; $max - $VALUE " | bc -l)
    echo " RESULT for s2 $score"
    echo
    rm -f temp
fi

if [ $command == 'gate' ] || [ $command == 'all' ]
then
    echo "****PINGING gateway "
    ping -c 10 172.29.4.177 > temp
    VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
    score=$(echo "scale=1; $max - $VALUE " | bc -l)
    echo " RESULT for gate $score"
    echo
    rm -f temp
fi

wget http://172.29.4.183:16280/64MB.bin -O /dev/null
wget http://172.29.4.187:16280/64MB.bin -O /dev/null

##############FIRST ROUND!!!!!!-------------------------------------------------- 
echo "****ROUND 1===================== "
echo

./break.sh

sleep 1m

if [ $command == 'vhost1' ] || [ $command == 'all' ]
then
echo "****PINGING VHOST 1 eth0"
ping -c 10 172.29.4.178 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht0 $score"
echo
rm -f temp


#echo "****PINGING VHOST 1 eth1"
#ping -c 10 172.29.4.180 > temp
#VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
#score=$(echo "scale=1; $max - $VALUE " | bc -l)
#echo " RESULT for vhost1 eht1 $score"
#echo
#rm -f temp


echo "****PINGING VHOST 1 eth2"
ping -c 10 172.29.4.184 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht2 $score"
echo
rm -f temp
fi



if [ $command == 'vhost2' ] || [ $command == 'all' ]
then
#echo "****PINGING VHOST 2 eth0"
#ping -c 10 172.29.4.181 > temp
#VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
#score=$(echo "scale=1; $max - $VALUE " | bc -l)
#echo " RESULT for vhost1 eht0 $score"
#echo
#rm -f temp


echo "****PINGING VHOST 2 eth1"
ping -c 10 172.29.4.182 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht1 $score"
echo
rm -f temp


echo "****PINGING VHOST 2 eth2"
ping -c 10 172.29.4.189 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht2 $score"
echo
rm -f temp
fi



if [ $command == 'vhost3' ] || [ $command == 'all' ]
then
echo "****PINGING VHOST 3 eth0"
ping -c 10 172.29.4.185 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht0 $score"
echo
rm -f temp


echo "****PINGING VHOST 3 eth1"
ping -c 10 172.29.4.186 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht1 $score"
echo
rm -f temp


echo "****PINGING VHOST 3 eth2"
ping -c 10 172.29.4.190 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht2 $score"
echo
rm -f temp
fi



if [ $command == 's1' ] || [ $command == 'all' ]
then
echo "****PINGING s1 "
ping -c 10 172.29.4.183 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for s1 $score"
echo
rm -f temp
fi


if [ $command == 's2' ] || [ $command == 'all' ]
then
echo "****PINGING s2 "
ping -c 10 172.29.4.187 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for s2 $score"
echo
rm -f temp
fi

if [ $command == 'gate' ] || [ $command == 'all' ]
then
echo "****PINGING gateway "
ping -c 10 172.29.4.177 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for gate $score"
echo
rm -f temp
fi

wget http://172.29.4.183:16280/64MB.bin -O /dev/null
wget http://172.29.4.187:16280/64MB.bin -O /dev/null


##############second ROUND!!!!!!0--------------------------------------------------

./FixNetwork.sh

sleep 15

echo "****ROUND 2===================== "
echo

if [ $command == 'vhost1' ] || [ $command == 'all' ]
then
echo "****PINGING VHOST 1 eth0"
ping -c 10 172.29.4.178 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht0 $score"
echo
rm -f temp


echo "****PINGING VHOST 1 eth1"
ping -c 10 172.29.4.180 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht1 $score"
echo
rm -f temp


echo "****PINGING VHOST 1 eth2"
ping -c 10 172.29.4.184 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht2 $score"
echo
rm -f temp
fi



if [ $command == 'vhost2' ] || [ $command == 'all' ]
then
echo "****PINGING VHOST 2 eth0"
ping -c 10 172.29.4.181 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht0 $score"
echo
rm -f temp


echo "****PINGING VHOST 2 eth1"
ping -c 10 172.29.4.182 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht1 $score"
echo
rm -f temp


echo "****PINGING VHOST 2 eth2"
ping -c 10 172.29.4.189 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht2 $score"
echo
rm -f temp
fi



if [ $command == 'vhost3' ] || [ $command == 'all' ]
then
echo "****PINGING VHOST 3 eth0"
ping -c 10 172.29.4.185 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht0 $score"
echo
rm -f temp


echo "****PINGING VHOST 3 eth1"
ping -c 10 172.29.4.186 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht1 $score"
echo
rm -f temp


echo "****PINGING VHOST 3 eth2"
ping -c 10 172.29.4.190 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht2 $score"
echo
rm -f temp
fi



if [ $command == 's1' ] || [ $command == 'all' ]
then
echo "****PINGING s1 "
ping -c 10 172.29.4.183 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for s1 $score"
echo
rm -f temp
fi


if [ $command == 's2' ] || [ $command == 'all' ]
then
echo "****PINGING s2 "
ping -c 10 172.29.4.187 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for s2 $score"
echo
rm -f temp
fi

if [ $command == 'gate' ] || [ $command == 'all' ]
then
echo "****PINGING gateway "
ping -c 10 172.29.4.177 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for gate $score"
echo
rm -f temp
fi

wget http://172.29.4.183:16280/64MB.bin -O /dev/null
wget http://172.29.4.187:16280/64MB.bin -O /dev/null

echo "****ROUND 3===================== "
echo

./vnltopo111.sh vhost1 setlossy eth2 100
./vnltopo111.sh vhost3 setlossy eth0 100

sleep 1m



if [ $command == 'vhost1' ] || [ $command == 'all' ]
then
echo "****PINGING VHOST 1 eth0"
ping -c 10 172.29.4.178 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht0 $score"
echo
rm -f temp


echo "****PINGING VHOST 1 eth1"
ping -c 10 172.29.4.180 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht1 $score"
echo
rm -f temp


#echo "****PINGING VHOST 1 eth2"
#ping -c 10 172.29.4.184 > temp
#VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
#score=$(echo "scale=1; $max - $VALUE " | bc -l)
#echo " RESULT for vhost1 eht2 $score"
#echo
#rm -f temp
fi



if [ $command == 'vhost2' ] || [ $command == 'all' ]
then
echo "****PINGING VHOST 2 eth0"
ping -c 10 172.29.4.181 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht0 $score"
echo
rm -f temp


echo "****PINGING VHOST 2 eth1"
ping -c 10 172.29.4.182 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht1 $score"
echo
rm -f temp


echo "****PINGING VHOST 2 eth2"
ping -c 10 172.29.4.189 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht2 $score"
echo
rm -f temp
fi



if [ $command == 'vhost3' ] || [ $command == 'all' ]
then
#echo "****PINGING VHOST 3 eth0"
#ping -c 10 172.29.4.185 > temp
#VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
#score=$(echo "scale=1; $max - $VALUE " | bc -l)
#echo " RESULT for vhost1 eht0 $score"
#echo
#rm -f temp


echo "****PINGING VHOST 3 eth1"
ping -c 10 172.29.4.186 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht1 $score"
echo
rm -f temp


echo "****PINGING VHOST 3 eth2"
ping -c 10 172.29.4.190 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht2 $score"
echo
rm -f temp
fi



if [ $command == 's1' ] || [ $command == 'all' ]
then
echo "****PINGING s1 "
ping -c 10 172.29.4.183 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for s1 $score"
echo
rm -f temp
fi


if [ $command == 's2' ] || [ $command == 'all' ]
then
echo "****PINGING s2 "
ping -c 10 172.29.4.187 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for s2 $score"
echo
rm -f temp
fi

if [ $command == 'gate' ] || [ $command == 'all' ]
then
echo "****PINGING gateway "
ping -c 10 172.29.4.177 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for gate $score"
echo
rm -f temp
fi

wget http://172.29.4.183:16280/64MB.bin -O /dev/null
wget http://172.29.4.187:16280/64MB.bin -O /dev/null




./FixNetwork.sh

sleep 15

echo "****ROUND 4===================== "
echo

if [ $command == 'vhost1' ] || [ $command == 'all' ]
then
echo "****PINGING VHOST 1 eth0"
ping -c 10 172.29.4.178 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht0 $score"
echo
rm -f temp


echo "****PINGING VHOST 1 eth1"
ping -c 10 172.29.4.180 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht1 $score"
echo
rm -f temp


echo "****PINGING VHOST 1 eth2"
ping -c 10 172.29.4.184 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht2 $score"
echo
rm -f temp
fi



if [ $command == 'vhost2' ] || [ $command == 'all' ]
then
echo "****PINGING VHOST 2 eth0"
ping -c 10 172.29.4.181 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht0 $score"
echo
rm -f temp


echo "****PINGING VHOST 2 eth1"
ping -c 10 172.29.4.182 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht1 $score"
echo
rm -f temp


echo "****PINGING VHOST 2 eth2"
ping -c 10 172.29.4.189 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht2 $score"
echo
rm -f temp
fi



if [ $command == 'vhost3' ] || [ $command == 'all' ]
then
echo "****PINGING VHOST 3 eth0"
ping -c 10 172.29.4.185 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht0 $score"
echo
rm -f temp


echo "****PINGING VHOST 3 eth1"
ping -c 10 172.29.4.186 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht1 $score"
echo
rm -f temp


echo "****PINGING VHOST 3 eth2"
ping -c 10 172.29.4.190 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for vhost1 eht2 $score"
echo
rm -f temp
fi



if [ $command == 's1' ] || [ $command == 'all' ]
then
echo "****PINGING s1 "
ping -c 10 172.29.4.183 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for s1 $score"
echo
rm -f temp
fi


if [ $command == 's2' ] || [ $command == 'all' ]
then
echo "****PINGING s2 "
ping -c 10 172.29.4.187 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for s2 $score"
echo
rm -f temp
fi

if [ $command == 'gate' ] || [ $command == 'all' ]
then
echo "****PINGING gateway "
ping -c 10 172.29.4.177 > temp
VALUE=$(cat temp | grep "% packet loss" -A1| head -1 | cut -d , -f 3| cut -d % -f 1)
score=$(echo "scale=1; $max - $VALUE " | bc -l)
echo " RESULT for gate $score"
echo
rm -f temp
fi

wget http://172.29.4.183:16280/64MB.bin -O /dev/null
wget http://172.29.4.187:16280/64MB.bin -O /dev/null

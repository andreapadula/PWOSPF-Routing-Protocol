Andrea Padula apadula@email.arizona.edu
Mridul Mohan Bharadwaj mridulbharadwaj@email.arizona.edu

ATTENTION BEFORE RUNNING THE PROGRAM SETLOSSY TO 0 FOR EVERYTHING otherwise it will not work

./vnltopoXXX.sh vhost1 setlossy eth0 0
./vnltopoXXX.sh vhost1 setlossy eth1 0
./vnltopoXXX.sh vhost1 setlossy eth2 0
./vnltopoXXX.sh vhost2 setlossy eth0 0
./vnltopoXXX.sh vhost2 setlossy eth1 0
./vnltopoXXX.sh vhost2 setlossy eth2 0
./vnltopoXXX.sh vhost3 setlossy eth0 0
./vnltopoXXX.sh vhost3 setlossy eth1 0
./vnltopoXXX.sh vhost3 setlossy eth2 0


 
The project in my testing works. The convergence time is :

20-30 sec the first time all routers are up and running
40-60 sec when a link fails. 

My testing were start all three vhost (vhost1 with .net and vhost2 and vhost3 with .empty).wait 20-30 second ping all the interfaces and ping servers and download a file from the two servers. Then break a link between vhost1 and vhost2 and ping all interface without the two that are down ping the servers and wget the file. Fix the link and ping everything and download the files from servers. Then break link from vhost1 and vhost3 and do the same and fix the link again. I included my test(./test.sh all) in the submission(not super beautiful , but I believe it tests everything). Nothing is being hardcoded so it should work with other topology as well. 
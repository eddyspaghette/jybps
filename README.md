# Just-Your-Basic-Port-Scanner (2.0)
A multithreaded port scanner - built using Boost.

Used primarily for CTFs, TryHackMe rooms, HackTheBox, etc.

**Use at your own discretion, this program does not support or promote unethical or illegal behavior in anyway. This is simply a free tool to help users with CTFs or other legal endeavors.**

## Prerequisites
Tested on g++ (Ubuntu 9.4.0-1ubuntu1~18.04) 9.4.0
Mileage may vary on other g++ versions

## Installation

For Unix Users
1. Clone this repository
2. Download the boost library [boost_1_77_0.tar.gz](https://boostorg.jfrog.io/ui/native/main/release/1.77.0/source/boost_1_77_0.tar.gz]) or on their [website](https://www.boost.org/) if you prefer.
3. Put this gz file inside the cloned repository.
4. Give run permissions to setup.sh and run the shell file.
```
chmod +x setup.sh && sudo ./setup.sh
```
6. An executable called jybps should be inside this repository. To run it from anywhere:
```
sudo ln -s $(pwd)/jybps /usr/bin/jybps
```
7. Congrats! Now you should be able to run it from anywhere.

## Usage
<img src="https://user-images.githubusercontent.com/60508293/140613358-ac1cb84f-0699-46e3-a2b7-0225ea58013a.png" height="350">



jybps takes 2 parameters: **\<domain\> [worker_multipler]**

e.g) jybps 10.123.13.12


e.g) jybps 10.123.13.12 5

1. Domain should be a FQN or IP address.
  
2. Worker_multipler by **default is 60**, it first calls hardware_concurrency to determine the number of threads on a given system
  then using this hint, we use the worker_multipler to determine the amount of workers.
  
3. These workers are then continuously run to grab jobs from the thread pool until no jobs are left.


## Improvements
You may be able to modify the TIMEOUT const value set in the connect_with_timeout() function to lower the scanning time further.
**default is 500000 = 0.5s** which may vary depending on your internet connection.

However be warned that the timeout is how long jybps will wait for a response until it assumes the port is closed.

Also depending on the hardware threads you have you may find that giving a lower/higher value for the worker_multipler parameter may improve results. For example, my system gives back 6 threads as the hint given by hardware_concurrency().

Note that creating threads are resource intensive and reusing them is a much better use of resources so **more threads does not equal more performance.**

To recreate the executable after changes simply type
```
make
```

## Why choose JYBPS over other port scanners?

nmap is great for enumerating but excessive for simply port scanning.  While nmap does support fast scans, it is tedious to remember the options and flags needed. jybps solves this by simply asking for a FQDN domain or ip to be passed in.

### A direct comparison vs Threader 3000

jybps do not give false results often. 

This, in conjunction with other multithreading port scanners giving false results (shown below on the Threader 3000 scan) led me to create jybps.

Let's take a comparison of a scan using another multithreaded port scanner (threader 3000)



<img src="https://user-images.githubusercontent.com/60508293/140613524-d9bd05f2-fccc-4332-b74f-b13cd4b2747a.png" height="350">

<img src="https://user-images.githubusercontent.com/60508293/140613358-ac1cb84f-0699-46e3-a2b7-0225ea58013a.png" height="350">

| jybps  | threader3000 |
| ------------- | ------------- |
| took 1:33  | took 1:40  |
| scanned all ports  | missed 3 ports |

If we use nmap to service scan these open ports then we can see that indeed threader3000 missed 3 ports.

<img src="https://user-images.githubusercontent.com/60508293/140614089-b4bb2785-2b7c-4170-a7f2-52085efc1d1f.png" height="350">

Additionally, jybps allows leverage of greater computing power by allowing users to modify the optional parameter in worker_multiplier.


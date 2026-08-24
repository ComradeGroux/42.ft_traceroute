# 42.ft_traceroute
A little reimplementation of the traceroute command in C for learning purpose.

## How it works
The traceroute commande take advantage of the TTL (time-to-live) field in the IP header.

Every router that receive a packet will decrease the TTL by 1 and then pass it over. When the TTL meets 0, the packet is destroy and the router respond with an ICMP message of type `ICMP_TIME_EXCEEDED`.

There are 3 different methods: UDP, TCP, ICMP. This implementation will use ICMP.

Now that we know that, it pretty easy to imagine how we can implement this command. We just need to:
* Construct an `ICMP_ECHO` request with a TTL value of 1.
* Send it over the network.
* Calculate statistics like the RTT (Round-trip time).
* Increase the TTL by 1 and restart
Receiving a respond of type `ICMP_ECHOREPLY` means that the destination was reach.

## Warning
This project use a `DGRAM` socket with `IPPROTO_ICMP`. This is the equivalent of a `RAW` socket with `IPPROTO_ICMP` but doesn't required root access. HOWEVER, your user's GID must be include in range of the system control `net.ipv4.ping_group_range MIN MAX`. To check that, you can:
```shell
id -g
sysctl -n net.ipv4.ping_group_range
```
To authorize every user:
`sudo sysctl -w net.ipv4.ping_group_range="0 2147483647"`
To make it reboot proof, you must add this line in `/etc/sysctl.conf` and then run `sudo sysctl --system`
`net.ipv4.ping_group_range= 0 2147483647`

## Use of AI
I did use Claude mainly for recapitulation on how traceroute works, discussion about how approch the problem and to have a "second" though on my ideas.
However, every lines of code was wrote by hand and I explicitely tell Claude to not write any code in the discussion. Pasting or copying code from an AI IS NOT how we learn.

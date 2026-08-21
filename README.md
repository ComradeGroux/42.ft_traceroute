# 42.ft_traceroute
A little reimplementation of the traceroute command in C for learning purpose

## How it works
The traceroute commande take advantage of the TTL (time-to-live) field in the IP header.
Every router that receive a packet will decrease the TTL by 1 and then pass it over. When the TTL meets 0, the packet is destroy and the router respond with an ICMP message of type ICMP_TIME_EXCEEDED.

Now that we know that, it pretty easy to imagine how we can implement this command. We just need to:

	Construct an ICMP_ECHO request with a TTL value of 1.
	Send it over the network.
	Calculate statistics like the RTT (Round-trip time).
	Increase the TTL by 1 and restart

Receiving a respond of type ICMP_ECHOREPLY means that the destination was reach.

## Warning
This project use a RAW socket. That means it must run in root. To avoid this, I could use a DGRAM socket but it is implementation depending so I choose to modify executable capabilities just after the link part so we need to root priviliege only one time during the compilation and then we can run the application in user space.

## Use of AI
I did use Claude for recapitulation on how traceroute works, to discuss about how approch the problem and to have a "second" though on my ideas.
However, every lines of code was wrote by hand and I explicitely tell Claude to not write code in the discussion. Pasting or copying code from an AI IS NOT how we learn.

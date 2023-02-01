
grabip
------

This is a simple routine that takes text data in via "stdin" and parses it for unique IP address.  When IPv4 or IPv6 addresses are found, it prints them out via stdout.

For example:

<pre>
$ echo "this is a IP 192.168.2.1.  This is an IP [192.168.2.2].  Here's some IPv6 stuff fe80::5054:ff:fec1:ce" | /usr/local/bin/grabip
192.168.2.1
192.168.2.2
fe80::5054:ff:fec1:ce
</pre>







grabip
------

This is a simple routine that takes text data in via "stdin" and parses it for unique IP addresses.  When IPv4 or IPv6 addresses are found, it prints them out via stdout.  

This tool can be used to create small "indices" of IPv4/IPv6 addresses from large text data sets.

For example:

<pre>
$ echo "this is a IP 192.168.2.1.  This is an IP [192.168.2.2].  Here's some IPv6 stuff fe80::5054:ff:fec1:ce" | /usr/local/bin/grabip
192.168.2.1
192.168.2.2
fe80::5054:ff:fec1:ce
</pre>


You can also take data in from a file: 

<pre>
$ cat /var/log/syslog | /usr/local/bin/grabip
127.0.0.1
62.233.50.2
185.225..247
10.27.2.99
62.77.172.87
142.98.10.71
112.171.162.85
185.181.143.71
177.190.218.244
191.32.162.159
61.177.171.51
94.60.251.225
192.241.109.74
68.132.87.241
...
</pre>







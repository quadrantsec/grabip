
grabip
------

This is a simple routine that takes text data in via "stdin" and parses it for any unique IP addresses.  When IPv4 or IPv6 addresses are found, it prints them out via stdout.  

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
185.225.1.247
10.27.2.99
62.77.172.87
142.98.10.71
112.171.162.85
185.181.143.71
177.190.218.244
...
</pre>

The normal use case is to be able to create IPv4/IPv6 indices for many log files.  For example,  lets say
you have a directory containing many days/weeks/months of files.  Like this:

<pre>
-rw-r--r-- 1 root root 297977 Dec 26 23:59 10.22.3.110-20221226.log.gz
-rw-r--r-- 1 root root 292392 Dec 27 23:59 10.22.3.110-20221227.log.gz
-rw-r--r-- 1 root root 295880 Dec 28 23:59 10.22.3.110-20221228.log.gz
-rw-r--r-- 1 root root 297221 Dec 29 23:59 10.22.3.110-20221229.log.gz
-rw-r--r-- 1 root root 229421 Dec 30 23:59 10.22.3.110-20221230.log.gz
-rw-r--r-- 1 root root 185357 Dec 31 23:59 10.22.3.110-20221231.log.gz
-rw-r--r-- 1 root root 184645 Jan  1 23:55 10.22.3.110-20230101.log.gz
-rw-r--r-- 1 root root 184402 Jan  2 23:55 10.22.3.110-20230102.log.gz
-rw-r--r-- 1 root root 182950 Jan  3 23:59 10.22.3.110-20230103.log.gz
-rw-r--r-- 1 root root 183591 Jan  4 23:55 10.22.3.110-20230104.log.gz
...
</pre>

We can use ``grabip`` to build IPv4/IPv6 indices based on those files, like this:

<pre>

$ for IN in *.gz; do zcat $IN | /usr/local/bin/grabip > $IN.ip.index; done

</pre>

You will now have a directory that looks similar to this:

<pre>

-rw-r--r-- 1 root root 297977 Dec 26 23:59 10.22.3.110-20221226.log.gz
-rw-r--r-- 1 root root    131 Dec 26 23:59 10.22.3.110-20221226.log.gz.ip.index
-rw-r--r-- 1 root root 292392 Dec 27 23:59 10.22.3.110-20221227.log.gz
-rw-r--r-- 1 root root    231 Dec 27 23:59 10.22.3.110-20221227.log.gz.ip.index
-rw-r--r-- 1 root root 295880 Dec 28 23:59 10.22.3.110-20221228.log.gz
-rw-r--r-- 1 root root    666 Dec 28 23:59 10.22.3.110-20221228.log.gz.ip.index
...

</pre>

These new ".ip.index" files will contain every IPv4 and IPv6 address contained within each
dataset. 

Now, rather than searching huge datasets for IP addresses, you can target only the files that
you've already pre-index'ed with ``grabip`` for that data.  For example:

<pre>
zgrep "192.168.1.222" `grep "192.168.1.222" *.ip.index | cut -d':' -f1 | sed 's/\.ip\.index//g'`
</pre>

This will _only_ search for "192.168.1.222" in files that we already know (have indexed) that
contain "192.168.1.222".

This results in faster search times as you aren't searching through files we know the data isn't present in. ``grabip`` is meant to be run after ``logrotate`` has completed its tasks. 

Installation
------------

``grabip`` has no prerequisites.  All you need is a C compiler.

<pre>

$ ./autogen.sh
$ ./configure 
$ make 
$ sudo make install

</pre>



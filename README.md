torrify
=======

A very simple tool to running multiple Tor instances (with separate configs).

The initial idea was to implement a tool that would allow me to use services
that are unavailable to my country via some proxy IP. The best choice of the tool
happened to be Tor and it was pretty good until I felt the need to have two
exit-nodes (in US and in UK) running simulataneously, or at least making the
switch between those more cofortable and less time consuming.

How to use
----------

Install the application. Add several torrc files like the one below. Have fun.

##### Torrc file samples:

    # 
    # torrc-us: special torrent file for US exit nodes
    #
    SocksPort 9050 # Default: Bind to localhost:9050 for local connections.
    ControlPort 9051
    ExitNodes {us}
    ExcludeNodes {a1},{??}
    StrictNodes 1

    
##### Todo

* add simple torrc file editor
* add tor logging visualization


Known issues
------------

High severity
* None

Medium severity
* Occasionaly the visualization of location and current ext. IP address i not up-to-date
* Adding two or more torrc with the same filename may lead to unpredictable behavior
* Adding two or more torrc with the same control port may lead to unpredictable behavior

Low severity
* The installer contains its own tor executable, so it may be not the recent version
* No confirmation on deleting the torrc file from list (watch out)


Useful documentation
--------------------

* Tor manual: https://www.torproject.org/docs/tor-manual.html.en
* Torctl howto: https://svn.torproject.org/svn/torctl/trunk/doc/howto.txt

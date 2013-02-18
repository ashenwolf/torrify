torrify
=======

A very simple tool to run multiple Tor instances (with separate configs).

The installer can be found here: http://graywolf.org.ua/download/torrify_setup.exe

_Note: the links to archives above will download only the source code._

The initial idea was to implement a tool that would allow me to use services
that are unavailable to my country via some proxy IP. The best choice of the tool
happened to be Tor and it was pretty good until I felt the need to have two
exit-nodes (in US and in UK) running simultaneously, or at least making the
switch between those more comfortable and less time consuming.

![Torrify screenshot](http://i.piccy.info/i7/dd525f70cdf2eb8a93495011859d1ac1/4-55-1735/42138161/torrify.png "Torrify screenshot")

How to use
----------

Install the application. Add several torrc files like the one below. Have fun.

##### Torrc file samples:

###### US exit node:

    # 
    # torrc-us: special torrent file for US exit nodes
    #
    SocksPort 9050 # Default: Bind to localhost:9050 for local connections.
    DataDirectory C:\Users\<username>\Application Data\Tor\us # should be different
                                                              # for different Tor instances
    ControlPort 9051
    ExitNodes {us}
    ExcludeNodes {a1},{??}
    StrictNodes 1
    
###### GB exit node:

    # 
    # torrc-gb: special torrent file for GB exit nodes
    #
    SocksPort 9052 # Default: Bind to localhost:9050 for local connections.
    DataDirectory C:\Users\<username>\Application Data\Tor\gb # should be different
                                                              # for different Tor instances
    ControlPort 9053
    ExitNodes {gb}
    ExcludeNodes {a1},{??}
    StrictNodes 1

##### Todo

* add simple torrc file editor
* hash password support
* add tor logging visualization


Known issues
------------

High severity
* None

Medium severity
* Occasionally the visualization of location and current ext. IP address i not up-to-date
* Adding two or more torrc with the same file name may lead to unpredictable behavior
* Adding two or more torrc with the same control port may lead to unpredictable behavior

Low severity
* The installer contains its own tor executable, so it may be not the recent version
* No confirmation on deleting the torrc file from list (watch out)

Acknowledgements
----------------

* @minus-one - testing and improvements suggestion

For contributors
--------------------

The software is written purely using Qt5 + msvc2010. Ports to Linux would be greatly appreciated.

##### Useful links

* Tor manual: https://www.torproject.org/docs/tor-manual.html.en
* Torctl howto: https://svn.torproject.org/svn/torctl/trunk/doc/howto.txt

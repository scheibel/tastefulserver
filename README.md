Tasteful Server
=========

Tasteful Server provides a mulithreaded server architecture for high performance network applications.
It is written in C++ and uses the Qt library.

You can specify a number of servers with their own handlers.
Those servers share a thread pool of user defined size to handle incoming data.

This library comes with a HTTP suite for handling incoming HTTP requests.
You can pass your own callback to handle those completely preparsed requests with an object oriented interface.
SSL for HTTPS is also supported.

Content
------------

* table of contents
{:toc}

Requirements
---------------------

 * Qt 4.8
 * C++11
 * CMake 2.6
 
Building Tasteful Server
-----------------------------------
 
    > mkdir build && cd build
    > cmake ..
    > make
 
Running Examples
---------------------------------

    > ./examples/HttpServer

And then open your favorite browser and navigate to [localhost:8080](http://localhost:8080).
Or for an Https-Server

    > ./examples/HttpsServer

with the URL [https://localhost:8080](https://localhost:8080)

Installation
----------------

The Tasteful Server library and its header files can be installed in your system using the following command:

### Linux

    > sudo make install

### Windows and MacOS

Windows and MacOS are not tested yet.

License
-----------

Tasteful Server is licensed under the LPGL-3.
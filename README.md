Tasteful Server
=========

Tasteful Server provides a mulithreaded server architecture for high performance network applications.
It is written in C++ and uses the Qt library.

You can specify a number of servers with their own handlers.
Those servers share a thread pool of user defined size to handle incoming data.

This library comes with a HTTP suite for handling incoming HTTP requests.
You can pass your own callback to handle those completely preparsed requests with an object oriented interface.
SSL for HTTPS is also supported.

As it is now, only Linux is tested as development and deployment environment, so no build information for Windows, MacOS or any other OS is provided.
Since all platform-dependent features of the Tasteful Server are provided through Qt, it should also run on any other OS to which Qt was ported.

Content
------------

 * [Features](#features)
 * [Requirements](#requirements)
 * [Building Tasteful Server](#building-tasteful-server)
 * [Running Examples](#running-examples)
 * [Installation](#installation)
 * [Known Projects Using Tasteful Server](#known-projects-using-tasteful-server)
 * [License](#license)

Features
-------------

 * Thread pool with user defined size
 * Multiple servers share one thread pool
 * HTTP server and HTTPS server
 * Handling of requests through callbacks
 * HTTP parser which translates requests to objects
 * Convenience support for several HTTP features
     * GET and POST parameters
     * Multipart and uploaded files
     * Cookies

Requirements
---------------------

 * [Qt 5.0](http://qt.nokia.com/)
 * [gcc 4.6](http://gcc.gnu.org/)
 * [CMake 2.6](http://www.cmake.org/)
 
Building Tasteful Server
-----------------------------------
 
    > mkdir build && cd build
    > cmake ..
    > make
 
Running Examples
---------------------------------

First you have to compile Tasteful Server with examples.

    > cmake .. -DWITH_EXAMPLES=ON
    > make

Then you can start the simple HTTP server.

    > ./examples/HttpServer

And then open your favorite browser and navigate to [localhost:8080](http://localhost:8080).  
For an Https-Server with the URL [https://localhost:8080](https://localhost:8080) you can start:

    > ./examples/HttpsServer

Installation
----------------

The Tasteful Server library and its header files can be installed in your system using the following command:

    > sudo make install

Known Projects Using Tasteful Server
--------------------------------------------------------

 * [Tasteful Framework](https://github.com/scheibel/tasteful-framework)

License
-----------

Tasteful Server is licensed under the LPGL-3.

# Modified Tor Instance
Provides modifications to Tor to emit incoming and outgoing cells to the cell logger.

TODO: implement cell emitter

## About
Tor protects your privacy on the internet by hiding the connection between
your Internet address and the services you use. We believe Tor is reasonably
secure, but please ensure you read the instructions and configure it properly.

## Build

To build Tor from source:

```
./configure
make
make install
```

To build Tor from a just-cloned git repository:

```
./autogen.sh
./configure
make
make install
```
## Resources

Home page:

- https://www.torproject.org/

Download new versions:

- https://www.torproject.org/download/download.html

Documentation, including links to installation and setup instructions:

- https://www.torproject.org/docs/documentation.html

Frequently Asked Questions:

- https://www.torproject.org/docs/faq.html


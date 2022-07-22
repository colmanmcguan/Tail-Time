# Tail Time

Tail Time is a website fingerprinting defense built on top of Walkie-Talkie. In Tail Time, communication between the client and the Tor Guard is done in half-duplex except for that an active request cannot block queued requests longer than the configured timeout value. Additionally, the cell sequences of website traces are padded using a reference trace to ensure similarity between different websites.

## Browser Extension
The browser extension is implemented using the Firefox webRequest API. Half-duplex communication and the request timeout are implemented here.

**Installing the Browser Extension**
* Navigate to [about:debugging](about:debugging) in the Firefox search bar
* Click This Firefox
* Click Load Temporary Add-on
* Navigate to the extension files and select manifest.json

**Configuring Tail Time**
* Click the icon on the browser's toolbar
* Check/uncheck the box to enable/disable Tail Time
* Set the desired timeout value

## Modified Tor Instance
The modified instance of Tor is used to emit cell sequences of a webpage trace to the cell logger. Tor is run in docker for this experiment.

## Tor Cell Logger
The cell logger captures and stores the cells emitted from Tor for each webpage trace in a csv file. For each webpage load, the cell logger is restarted. It takes a single parameter -- the name of the file to write to.

## Controller
The controller sets up Tor, the cell logger, and webpage accesses to collect data for each trace.

TODO: implement main control script

## Trace Padding
Padding is added to cell sequences to make traces for different websites appear similar.

TODO: implement padding

## Reference
J. Liang, C. Yu, K. Suh and H. Han, "Tail Time Defense Against Website Fingerprinting Attacks," in IEEE Access, vol. 10, pp. 18516-18525, 2022, doi: [10.1109/ACCESS.2022.3146236](https://doi.org/10.1109/ACCESS.2022.3146236).

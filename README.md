# Tail Time

Tail Time is a website fingerprinting defense built on top of Walkie-Talkie.

## Browser Extension
The browser extension is implemented using the Firefox webRequest API. Half-duplex communication and the request timeout are implemented here.

The Tail Time extension includes a popup for easy configuration.
* Click the icon on the browser's toolbar
* Check/uncheck the box to enable/disable Tail Time
* Set the desired timeout value

## Modified Tor Instance
The modified instance of Tor is used to log cell sequences of a website trace.

TODO: implement cell logger

## Trace Padding
Padding is added to cell sequences to make traces for different websites appear similar.

TODO: implement padding

## Installing the Browser Plugin
* Navigate to about:debugging in the Firefox search bar. 
* Click This Firefox
* Load Temporary Add-on
* Navigate to the plugin files and select manifest.json

## Reference
J. Liang, C. Yu, K. Suh and H. Han, "Tail Time Defense Against Website Fingerprinting Attacks," in IEEE Access, vol. 10, pp. 18516-18525, 2022, doi: 10.1109/ACCESS.2022.3146236.

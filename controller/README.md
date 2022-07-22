##Controller

The controller sets up Tor, the cell logger, and accesses each webpage for data collection. 

The script visit.py creates a new Firefox profile for each webpage access and configures it to use the local Tor proxy. It takes 2 arguments -- the webpage to access (preceeded by http://) and a browser extension. Browser extensions are stored in the ext-config folder.

## Controller

The controller sets up Tor, the cell logger, and accesses each webpage for data collection. 

The script visit.py creates a new Firefox profile for each webpage access and configures it to use the local Tor proxyusing selenium via geckodriver. It takes 2 arguments -- the webpage to access (preceeded by http://) and a browser extension. Browser extensions are stored in the ext-config folder. 

# Requirements

**selenuium**
```python
python3 -m pip install selenium
```

**geckodriver**
Download from https://github.com/mozilla/geckodriver/releases

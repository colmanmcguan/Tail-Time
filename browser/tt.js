"use strict";

var data = {};      	/* timestamp request was sent, grouped by tab */
var ttt = {};       	/* tab's tail time (intervals) :) */
var config = {      	/* default config */
	mode: "wt/tt",
	timeout: 2000
};

function getConfig() {
	return JSON.stringify(config);
}

function setConfig(newConfig) {
	newConfig = JSON.parse(newConfig);

	if ((config.mode == "wt" || config.mode == "wt/tt") 
		&& newConfig.mode == "none") disable();

	config = newConfig;
	console.log("[*] New config loaded with values:\n"
			+ "\tmode: " + config.mode + "\n"
			+ "\ttimeout: " + config.timeout);
}

/* disable tt/wt */
function disable() {
	/* all blocked requests can be let go */
	for (var tab in data) {
		for (var req in tab) 
			if (typeof req == "function") req ();

		clearInterval(tab);
		delete data[tab];
	}
	console.log("[!] TT disabled")
}

/* flush out overdue requests */
function flush(tab) {
	var overdue = false;
	var t = new Date().getTime();

	for (var req in data[tab]) {
		/* remove requests older than tt */
		if (typeof data[tab][req] == "number" && t - data[tab][req] > config.timeout) {
			console.log("[*] Tab " + tab + ": Overdue request " + req 
					+ " cleared from outstanding requests: "
					+ (t - data[tab][req]) + "ms");
			delete data[tab][req];
			overdue = true;
		}
	}

	/* see if we can clear any blocked requests */
	if (overdue) clearBlocked(tab);
}

/* send any blocked requests, if able */
function clearBlocked(tab) {
	/* nothing to do */
	if (Object.keys(data[tab]).length == 0) return;

	for (var req in data[tab])
		/* still have requests out */
		if (typeof data[tab][req] == "number") return;

	/* no active requests, send blocked */
	console.log("[*] Tab " + tab + ": Sending burst of " + Object.keys(data[tab]).length 
			+ " blocked requests");
	for (var req in data[tab]) {
		data[tab][req] ({});
		data[tab][req] = new Date().getTime();
	}
}

/* need to cancel any blocked requests on this tab */
function tabRemoved(tab) {
	for (var req in data[tab]) 
		if (typeof data[tab][req] == "function") 
			data[tab][req]({cancel: true});

	clearInterval(ttt[tab]);
	delete ttt[tab];
	delete data[tab];
}

/* main function for receiving requests.
 * processed based on the current state of tt */
function reqNew(details) {
	var req = details.requestId;
	var tab = details.tabId;

	/* wt and/or tt disabled, don't burst requests */
	if (config.mode == "none")
		return;

	/* new webpage being loaded. */
	/* cancel blocked requests and destroy old interval */
	if (typeof details.documentUrl == "undefined" && data[tab]) {
		console.log("[*] Tab " + tab + ": Canceling blocked requests");
		tabRemoved(tab);
	}

	/* first request of a webpage. 
	 * cleanup current interval and data */
	if (!data[tab] && typeof details.documentUrl == "undefined") {
		data[tab] = {};

		/* only set up interval for tt */
		if (config.mode == "wt/tt") {
			console.log("[*] Tab " + tab + ": Setting up interval for new webpage load");
			ttt[tab] = setInterval(function() {flush(tab)}, 500);
		}

		/* always send first request */
		data[tab][req] = new Date().getTime();
		return;
	}

	/* check for outstanding requests before sending */
	for (var curReq in data[tab]) {
		if (typeof data[tab][curReq] == "number") {
			/* halt request until outstanding return or tt */
			return new Promise((resolve, reject) => {
				data[tab][req] = resolve;
			});
		}
	}

	/* need to check tab is still in data, otherwise there are
	 * errors in the console. i.e. the tab was closed just 
	 * before this request was sent */
	if (data[tab])
		/* nothing outstanding. this request can be sent */
		data[tab][req] = new Date().getTime();
}

/* function for receiving completed requests */
function reqCompleted(details) {
	var t = new Date().getTime();
	var req = details.requestId;
	var tab = details.tabId;

	if (config.mode == "none")
		return;

	/* delete request from data if it hasn't been already */
	if (data[tab] && data[tab][req]) {
		delete data[tab][req];

		/* check if any blocked requests can be sent */
		clearBlocked(tab);
	}
}

/* listen for new requests */
browser.webRequest.onBeforeRequest.addListener(
	reqNew, 
	{urls: ["<all_urls>"]}, 
	["blocking"]
);

browser.webRequest.onHeadersReceived.addListener((details) => {
	if (details.method == "HEAD" || details.method == "OPTIONS")
		reqCompleted(details);
	},
	{urls: ["<all_urls>"]}
);

/* listen for redirect */
browser.webRequest.onBeforeRedirect.addListener(
	reqCompleted,
	{urls: ["<all_urls>"]}
);

/* listen for completed requests */
browser.webRequest.onCompleted.addListener(
	reqCompleted,
	{urls: ["<all_urls>"]}
);

/* same as completed */
browser.webRequest.onErrorOccurred.addListener(
	reqCompleted,
	{urls: ["<all_urls>"]}
);

browser.tabs.onRemoved.addListener((tabId, removeInfo) => {
	console.log("[!] Tab " + tabId + ": closed");
	tabRemoved(tabId);
});

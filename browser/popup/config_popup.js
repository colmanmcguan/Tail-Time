"use-strict"

/* load in whatever the current config is */
var ffwindow = browser.extension.getBackgroundPage();
var config = JSON.parse(ffwindow.getConfig());

switch (config.mode) {
	case "none":
		document.getElementById("disable").checked = true;
		break;
	case "wt":
		document.getElementById("wt").checked = true;
		break;
	case "wt/tt":
		document.getElementById("wt/tt").checked = true;
		break;
}

document.getElementById("timeout").value = config.timeout;

/* listen for changes */
document.getElementById("upload").addEventListener("click", (e) => {
	var newMode = "";
	var newTimeout = parseInt(document.getElementById("timeout").value);

	/* catch invalid timeout values */
	if (newTimeout < 0 || isNaN(newTimeout)) 
		newTimeout = 0;

	/* somebody disabled tt but provided a timeout value anyways.. */
	if (!document.getElementById("wt/tt").checked)
		newTimeout = 0;

	if (document.getElementById("disable").checked)
		newMode = "none";
	else if (document.getElementById("wt").checked)
		newMode = "wt";
	else
		newMode = "wt/tt";

	var config = {
		mode: newMode,
		timeout: newTimeout
	};

	/* set the new config */
	ffwindow.setConfig(JSON.stringify(config));
});


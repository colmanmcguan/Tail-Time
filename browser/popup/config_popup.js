/* load in whatever the current config is */
var ffwindow = browser.extension.getBackgroundPage();

var config = JSON.parse(ffwindow.getConfig());

document.getElementById("enabled").checked = config.enabled;
document.getElementById("timeout").value = config.timeout;

/* listen for changes */
document.getElementById("upload").addEventListener("click", (e) => {
    newTimeout = parseInt(document.getElementById("timeout").value);

    /* catch invalid timeout values */
    if (newTimeout < 0 || isNaN(newTimeout)) 
        newTimeout = 0;

    /* somebody disabled tt but provided a timeout value anyways.. */
    if (!document.getElementById("enabled").checked)
        newTimeout = 0;

    config = {
        enabled: document.getElementById("enabled").checked,
        timeout: newTimeout
    };

    /* set the new config */
    ffwindow.setConfig(JSON.stringify(config));
});


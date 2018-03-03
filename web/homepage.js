document.getElementById("DevicesPage").style.display = "none";
document.getElementById("ProgramPage").style.display = "none";
document.getElementById("TimingPage").style.display = "none";

/* Dictionary that translates ids to webpages*/
var pages = {
    "Welcome": "WelcomePage",
    "DeviceMenuItem": "DevicesPage",
    "ControlMenuItem": "ProgramPage",
    "TimingMenuItem": "TimingPage"
};
var activePage = "WelcomePage";

/* Function called when a button in the top menu is clicked */
function changeActivePage(listElement) {
    var id = listElement.id;
    clearCurrentPage();
    var element = document.getElementById(pages[id]);
    element.style.display = "block";
    listElement.classList.add("topMenuActive");

    if (pages[id] === "DevicesPage")
    {
        deviceRefreshTimer = setInterval(setupDevicePage, 500);
        console.log('device page');
        setupDevicePage();
    }
    else {
        if (deviceRefreshTimer) {
            clearInterval(deviceRefreshTimer);
        }
    }

    if (pages[id] === "TimingPage") {
        requestSetupTimingPage();
    }

    activePage = pages[id];
}

/* Function to clear the current active page */
function clearCurrentPage() {
    for (item in pages) {
        var id = pages[item];
        var element = document.getElementById(id);
        element.style.display = "none";


        var listElement = document.getElementById(item);
        if (listElement) {
            if (listElement.classList.contains("topMenuActive")) {
                listElement.classList.remove("topMenuActive");
            }
        }

    }
}


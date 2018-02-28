document.getElementById("DevicesPage").style.display = "none";
document.getElementById("ProgramPage").style.display = "none";

/* Dictionary that translates */
var pages = {
    "Welcome": "WelcomePage",
    "DeviceMenuItem": "DevicesPage",
    "ControlMenuItem": "ProgramPage"
};
var activePage = "WelcomePage";

/* Model for a device info */
var deviceModel = {
    "name" : "MyDevice",
    "type" : "Switch",
    "id"   : 1,

    "relayStatus" : true,

    "hasPIR": true,
    "pirStatus": false,

    "switchStatus": false
};

/* Model for a switch event */
function Requisition(type, newStatus, targetId) {
    this.type = type;
    this.status = newStatus;
    this.target = targetId
}

/* Model for a device */

var deviceRefreshTimer;
var ignoreCheck = false;

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

    activePage = pages[id];
}

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

function setupDevicePage() {
    if (ignoreCheck) {
        return;
    }
    var element = document.getElementById("DevicesPage");

    var htmlForDeviceTemplate = "" +
        "        <div class=\"col-2\">\n" +
        "            <div class=\"deviceInfo\">\n" +
        "\n" +
        "                <div class=\"device row slightMargin\">\n" +
        "                    <div class=\"device labelNormal\">Name:</div>\n" +
        "                    <div id=\"NameValue\" class=\"device valueNormal\">Switch</div>\n" +
        "                    <br>\n" +
        "                </div>\n" +
        "\n" +
        "                <div class=\"device row slightMargin\">\n" +
        "                    <div class=\"device labelNormal\">Type:</div>\n" +
        "                    <div id=\"TypeValue\" class=\"device valueNormal\">Switch</div>\n" +
        "                    <br>\n" +
        "                </div>\n" +
        "\n" +
        "                <div class=\"device row slightMargin\">\n" +
        "                    <div class=\"device labelNormal\">ID:</div>\n" +
        "                    <div id=\"IdValue\" class=\"device valueNormal\">Switch</div>\n" +
        "                </div>\n" +
        "\n" +
        "                <div id=\"PlugInfo\" class=\"device row slightMargin\">\n" +
        "                    <div class=\"device labelNormal\">Power status:</div>\n" +
        "                    <!-- Rounded switch -->\n" +
        "                    <div class=\"device valueNormal\">\n" +
        "                        <label class=\"switch\"><input id=\"RelayStatus\" type=\"checkbox\"><span class=\"slider round\"></span></label>\n" +
        "                    </div>\n" +
        "                </div>\n" +
        "\n" +
        "                <div id=\"SwitchInfo\" class=\"device row slightMargin\">\n" +
        "                    <div class=\"device labelNormal\">Switch status:</div>\n" +
        "                    <!-- Rounded switch -->\n" +
        "                    <div class=\"device valueNormal\">\n" +
        "                        <label class=\"switch\"><input id=\"SwitchStatus\" type=\"checkbox\"><span class=\"slider round\"></span></label>\n" +
        "                    </div>\n" +
        "                </div>\n" +
        "\n" +
        "                <div id=\"PIRInfo\" class=\"device row slightMargin\">\n" +
        "                    <div class=\"device labelNormal\">Infrared status:</div>\n" +
        "                    <!-- Rounded switch -->\n" +
        "                    <div class=\"device valueNormal\">\n" +
        "                        <label id=\"PIRStatus\" class=\"pir \"></label>\n" +
        "                    </div>\n" +
        "                </div>\n" +
        "            </div>\n" +
        "        </div>";

    var requestForDevices = new XMLHttpRequest();
    requestForDevices.onreadystatechange = function () {
        if (this.readyState === 4 && this.status === 200) {
            element.innerHTML = "";
            var listOfDevices = JSON.parse(this.responseText);
            var amountOfDevices = listOfDevices.devices.length;
            var htmlForNewDevice;
            var device;
            for (var i = 0; i < amountOfDevices; i++) {
                device = listOfDevices.devices[i];
                htmlForNewDevice = document.createElement('template');
                htmlForNewDevice.innerHTML = htmlForDeviceTemplate.trim();
                htmlForNewDevice.content.getElementById("NameValue").innerHTML = device.name;
                htmlForNewDevice.content.getElementById("TypeValue").innerHTML = device.type;
                htmlForNewDevice.content.getElementById("IdValue").innerHTML = device.id;

                if (device.type === "Plug") {
                    htmlForNewDevice.content.getElementById("SwitchInfo").style.display = "none";
                    var relayStatus = htmlForNewDevice.content.getElementById("RelayStatus");
                    relayStatus.checked = device.relayStatus;
                    relayStatus.onclick = (function (id, elem) {
                        return function () {
                            console.log("Disabling device check");
                            ignoreCheck = true;
                            var req = new Requisition(1, elem.checked, id);
                            var jsonStr = JSON.stringify(req);
                            var ajaxReq = new XMLHttpRequest();
                            ajaxReq.onreadystatechange = function () {
                                var tim = setTimeout(function () {
                                    ignoreCheck = false;
                                    console.log("Enabling device check");
                                }, 1000);
                            };
                            ajaxReq.open("POST", "/cgi/issue_action", true);
                            ajaxReq.send(jsonStr);
                        }
                    })(device.id, relayStatus);
                    if (device.hasPir) {
                        if (device.pirStatus === true) {
                            htmlForNewDevice.content.getElementById("PIRStatus").classList.toggle("pirActive");
                        }
                    }
                    else {
                        htmlForNewDevice.content.getElementById("PIRInfo").style.display = "none";
                    }
                } else {
                    htmlForNewDevice.content.getElementById("RelayInfo").style.display = "none";
                    htmlForNewDevice.content.getElementById("SwitchStatus").checked = device.switchStatus;
                    htmlForNewDevice.content.getElementById("PIRInfo").style.display = "none";
                }

                htmlForNewDevice.content.getElementById("RelayStatus").checked = device.relayStatus;
                element.appendChild(htmlForNewDevice.content.firstChild);
            }
        }
    };
    requestForDevices.open("GET", "/cgi/devices", true);
    requestForDevices.send();
}
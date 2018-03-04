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

/* Device refresh timer */
var deviceRefreshTimer;
var ignoreCheck = false;

/* Device page setup function */
function setupDevicePage() {
    if (ignoreCheck) {
        return;
    }
    var element = document.getElementById("DevicesPage");
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
                            ajaxReq.setRequestHeader('Content-Type', 'application/json');
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
    requestForDevices.open("GET", "/cgi/devices_list", true);
    requestForDevices.send();
}
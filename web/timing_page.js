function requestSetupTimingPage() {
    var page = document.getElementById("TimingPage");
    var requestForDevices = new XMLHttpRequest();
    requestForDevices.onreadystatechange = setupTimingPage;
    requestForDevices.open("GET", "/cgi/devices_timing", true);
    requestForDevices.send()
}

function setupTimingPage() {
    if (this.readyState !== 4 || this.status !== 200) return;

    var listOfDevices = JSON.parse(this.responseText);
    var amountOfDevices = listOfDevices.length;
    var newHtml = "";
    var page = document.getElementById("TimingPage");
    page.innerHTML = "";
    for (var deviceNumber = 0; deviceNumber < amountOfDevices; deviceNumber++) {
        var device = listOfDevices[deviceNumber];
        var htmlForNewDevice = document.createElement('template');
        htmlForNewDevice.innerHTML = htmlForDeviceTimingTemplate.trim();

        htmlForNewDevice.content.getElementById("deviceName").innerHTML = device.name;

        var times = device.times;
        var amountOfTimes = times.length;
        var referenceList = [];
        for (var timeEntryNumber = 0; timeEntryNumber < amountOfTimes; timeEntryNumber++) {
            var timeEntry = times[timeEntryNumber];

            htmlForNewDevice.content.getElementById("SwitchStatus"+(timeEntryNumber+1)).checked = timeEntry.enabled;
            htmlForNewDevice.content.getElementById("Hour"+(timeEntryNumber+1)).value = parseInt(timeEntry.time/60);
            htmlForNewDevice.content.getElementById("Minute"+(timeEntryNumber+1)).value = timeEntry.time%60;
            if (timeEntry.action===true) {
                htmlForNewDevice.content.getElementById("activate"+(timeEntryNumber+1)).checked=true;
                htmlForNewDevice.content.getElementById("deactivate"+(timeEntryNumber+1)).checked=false;
            }
            else {
                htmlForNewDevice.content.getElementById("deactivate"+(timeEntryNumber+1)).checked=true;
                htmlForNewDevice.content.getElementById("activate"+(timeEntryNumber+1)).checked=false;
            }
            htmlForNewDevice.content.getElementById("activate"+(timeEntryNumber+1)).name = "action"+(timeEntryNumber+1)+device.id;
            htmlForNewDevice.content.getElementById("deactivate"+(timeEntryNumber+1)).name = "action"+(timeEntryNumber+1)+device.id;
            var reference = {
                'enabled': htmlForNewDevice.content.getElementById("SwitchStatus"+(timeEntryNumber+1)),
                'hour': htmlForNewDevice.content.getElementById("Hour"+(timeEntryNumber+1)),
                'minute': htmlForNewDevice.content.getElementById("Minute"+(timeEntryNumber+1)),
                'action': htmlForNewDevice.content.getElementById("activate"+(timeEntryNumber+1))
            };
            referenceList.push(reference);

        }
        htmlForNewDevice.content.getElementById("submitEntry").onclick = updateTimingFactory(device.id, referenceList);
        page.appendChild(htmlForNewDevice.content.firstChild);
    }
}

function updateTimingFactory(id, references) {
    return function () {
        var updateRequest =new XMLHttpRequest();
        var listOfEntries = [];

        for (var timingEntryNumber = 0; timingEntryNumber < references.length; timingEntryNumber++) {
            var enabled = references[timingEntryNumber].enabled.checked;
            var hours = parseInt(references[timingEntryNumber].hour.value);
            var minutes = parseInt(references[timingEntryNumber].minute.value);
            var timeVal = ((60*hours) + minutes).toString();
            var action = references[timingEntryNumber].action.checked;

            var timingEntry = {
                "enabled": enabled,
                "time": timeVal,
                "action": action
            };
            listOfEntries.push(timingEntry);
        }
        updateRequest.onreadystatechange = function () {
        };
        updateRequest.open("POST", "/cgi/update_timing", true);
        var data = {
            "id": id,
            "table": listOfEntries
        };
        var jsonstr = JSON.stringify(data);
        updateRequest.send(jsonstr);
        console.log(jsonstr);
    }
}
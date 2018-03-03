var deviceTemplate1 = {
    "name"      : "name1",
    "id"        : "123456678",
    "times"     : [
        {"enabled": true, "time": "60", "action":false},
        {"enabled": false, "time": "120", "action":true},
        {"enabled": true, "time": "180", "action":false},
        {"enabled": true, "time": "240", "action":false}
    ]
};
var deviceTemplate2 = {
    "name"      : "name2",
    "id"        : "4235",
    "times"     : [
        {"enabled": true, "time": "30", "action":false},
        {"enabled": false, "time": "130", "action":false},
        {"enabled": false, "time": "245", "action":true},
        {"enabled": true, "time": "300", "action":false}
    ]
};
var htmlForDeviceTimingTemplate = "\n" +
    "        <div class=\"col-4\">\n" +
    "            <!-- Rounded switch -->\n" +
    "            <div class=\"deviceTiming\">\n" +
    "                <p>Timing for <span id=\"deviceName\">Device 1</span></p>\n" +
    "\n" +
    "                <div class=\"timingEntry row\">\n" +
    "                    <div class=\"timingSwitch\">\n" +
    "                        <label class=\"switch\"><input id=\"SwitchStatus1\" type=\"checkbox\"><span class=\"slider round\"></span></label> Enable\n" +
    "                    </div>\n" +
    "                    <div class=\"timingTime\">\n" +
    "                        <input id=\"Hour1\" class=\"timeInput\"> h\n" +
    "                        <input id=\"Minute1\" class=\"timeInput\"> min\n" +
    "                    </div>\n" +
    "                    <div class=\"timingAction\">\n" +
    "                        <label class=\"container\">ON\n" +
    "                            <input type=\"radio\" name=\"action1\" id=\"deactivate1\">\n" +
    "                            <span class=\"checkmark\"></span>\n" +
    "                        </label>\n" +
    "                        <label class=\"container\">OFF\n" +
    "                            <input type=\"radio\" name=\"action1\" checked=\"checked\" id=\"activate1\">\n" +
    "                            <span class=\"checkmark\"></span>\n" +
    "                        </label>\n" +
    "                    </div>\n" +
    "                </div>\n" +
    "\n" +
    "                <div class=\"timingEntry row\">\n" +
    "                    <div class=\"timingSwitch\">\n" +
    "                        <label class=\"switch\"><input id=\"SwitchStatus2\" type=\"checkbox\"><span class=\"slider round\"></span></label> Enable\n" +
    "                    </div>\n" +
    "                    <div class=\"timingTime\">\n" +
    "                        <input id=\"Hour2\" class=\"timeInput\"> h\n" +
    "                        <input id=\"Minute2\" class=\"timeInput\"> min\n" +
    "                    </div>\n" +
    "                    <div class=\"timingAction\">\n" +
    "                        <label class=\"container\">ON\n" +
    "                            <input type=\"radio\" name=\"action2\" id=\"deactivate2\">\n" +
    "                            <span class=\"checkmark\"></span>\n" +
    "                        </label>\n" +
    "                        <label class=\"container\">OFF\n" +
    "                            <input type=\"radio\" name=\"action2\" checked=\"checked\" id=\"activate2\">\n" +
    "                            <span class=\"checkmark\"></span>\n" +
    "                        </label>\n" +
    "                    </div>\n" +
    "                </div>\n" +
    "\n" +
    "                <div class=\"timingEntry row\">\n" +
    "                    <div class=\"timingSwitch\">\n" +
    "                        <label class=\"switch\"><input id=\"SwitchStatus3\" type=\"checkbox\"><span class=\"slider round\"></span></label> Enable\n" +
    "                    </div>\n" +
    "                    <div class=\"timingTime\">\n" +
    "                        <input id=\"Hour3\" class=\"timeInput\"> h\n" +
    "                        <input id=\"Minute3\" class=\"timeInput\"> min\n" +
    "                    </div>\n" +
    "                    <div class=\"timingAction\">\n" +
    "                        <label class=\"container\">ON\n" +
    "                            <input type=\"radio\" name=\"action3\" id=\"deactivate3\">\n" +
    "                            <span class=\"checkmark\"></span>\n" +
    "                        </label>\n" +
    "                        <label class=\"container\">OFF\n" +
    "                            <input type=\"radio\" name=\"action3\" checked=\"checked\" id=\"activate3\">\n" +
    "                            <span class=\"checkmark\"></span>\n" +
    "                        </label>\n" +
    "                    </div>\n" +
    "                </div>\n" +
    "\n" +
    "                <div class=\"timingEntry row\">\n" +
    "                    <div class=\"timingSwitch\">\n" +
    "                        <label class=\"switch\"><input id=\"SwitchStatus4\" type=\"checkbox\"><span class=\"slider round\"></span></label> Enable\n" +
    "                    </div>\n" +
    "                    <div class=\"timingTime\">\n" +
    "                        <input id=\"Hour4\" class=\"timeInput\"> h\n" +
    "                        <input id=\"Minute4\" class=\"timeInput\"> min\n" +
    "                    </div>\n" +
    "                    <div class=\"timingAction\">\n" +
    "                        <label class=\"container\">ON\n" +
    "                            <input type=\"radio\" name=\"action4\" id=\"deactivate4\">\n" +
    "                            <span class=\"checkmark\"></span>\n" +
    "                        </label>\n" +
    "                        <label class=\"container\">OFF\n" +
    "                            <input type=\"radio\" name=\"action4\" checked=\"checked\" id=\"activate4\">\n" +
    "                            <span class=\"checkmark\"></span>\n" +
    "                        </label>\n" +
    "                    </div>\n" +
    "                </div>\n" +
    "\n" +
    "                <div id='submitEntry' class=\"timingEntry submit\">Submit</div>\n" +
    "            </div>\n" +
    "        </div>";

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
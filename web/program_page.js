var device1 =  {
    "id": "1234",
    "name": "switch"
};
var device2 =  {
    "id": "3256",
    "name": "socket"
};

var rule1 = {
    "triggerid": "1234",
    "triggerevent": true,
    "targetid": "1234",
    "targetact": true
};
var rule2 = {
    "triggerid": "3256",
    "triggerevent": false,
    "targetid": "1234",
    "targetact": true
};
var rule3 = {
    "triggerid": "1234",
    "triggerevent": true,
    "targetid": "3256",
    "targetact": false
};

var knownDevicesList = [device1, device2];
var knownRules = [rule1, rule2, rule3];
var ruleElementList = [];
var rulesOnDisplay = [];



function getDeviceById(id) {
    for (var i = 0; i < retrievedDevices.length; i++)
    {
        var device = retrievedDevices[i];
        if (device.id === id)
        {
            return device;
        }
    }
}

function getIdByName(name) {
    for (var i = 0; i < retrievedDevices.length; i++)
    {
        var device = retrievedDevices[i];
        if (device.name === name)
        {
            return device.id;
        }
    }
}

function createNewRuleHtml() {
    var elementFromModel = document.createElement("template");
    elementFromModel.innerHTML = htmlModelForRule.trim();
    var currentId;
    if (rulesOnDisplay.length === 0) {
        currentId = 0;
    }
    else
    {
        currentId = rulesOnDisplay[rulesOnDisplay.length-1]+1;
    }

    var ruleContainer = elementFromModel.content.getElementById("newContainer");
    ruleContainer.id = "ruleContainer"+currentId;

    var radioButtons = ruleContainer.getElementsByClassName("programradio");

    for (var radioIdx = 0; radioIdx < radioButtons.length; radioIdx++){
        radioButtons[radioIdx].name += currentId;
        radioButtons[radioIdx].id += currentId;
    }

    var drpdowns = ruleContainer.getElementsByClassName("dropdown-content");

    /* Fill the dropdowns */
    for (var drpIndex = 0; drpIndex < drpdowns.length; drpIndex++){
        var drpdown = drpdowns[drpIndex];
        drpdown.parentNode.getElementsByTagName("span")[0].id += currentId;
        for (var devIndex = 0; devIndex < retrievedDevices.length; devIndex++) {
            var devP = document.createElement("p");
            devP.innerText = retrievedDevices[devIndex].name;
            devP.onclick = function(elem, name) {
                return function () {
                    elem.parentNode.getElementsByTagName("span")[0].innerText = name;
                }
            } (drpdown, devP.innerText);
            drpdown.appendChild(devP);
        }
    }

    var page = document.getElementById("ProgramPage");
    page.appendChild(ruleContainer);
    rulesOnDisplay.push(currentId);
    return ruleContainer;
}

function clearAllRules() {
    var page = document.getElementById("ProgramPage");
    for (var ruleId=0; ruleId < rulesOnDisplay.length; ruleId++)
    {
        page.removeChild(document.getElementById("ruleContainer"+ruleId));
    }
    rulesOnDisplay = [];
}

function refreshRules() {
    var rulesReq = new XMLHttpRequest();
    rulesReq.onreadystatechange = function (ev) {
        if (this.readyState !== 4 || this.status !== 200) {
            return;
        }
        var receivedRules = JSON.parse(this.responseText);
        clearAllRules();
        receivedRules.forEach(
            function (value, index) {
                createNewRuleHtml();
                document.getElementById("triggerSelected"+index).innerText = getDeviceById(value.triggerid).name;
                document.getElementById("targetSelected"+index).innerText = getDeviceById(value.targetid).name;

                document.getElementById("triggered"+index).checked = value.triggerevent;
                document.getElementById("untriggered"+index).checked = !value.triggerevent;
                document.getElementById("activate"+index).checked = value.targetact;
                document.getElementById("deactivate"+index).checked = !value.targetact;

            }
        )
    };
    rulesReq.open("GET", "/cgi/request_rules", true);
    rulesReq.send();
}

function ruleObject(trigger_id, trigger, target_id, action){
    this.triggerid = trigger_id;
    this.triggerevent = trigger;
    this.targetid = target_id;
    this.targetact = action;
}

function sendRules() {
    var rules = [];
    for (var i = 0; i < rulesOnDisplay.length; i++)
    {
        var ruleId = rulesOnDisplay[i];
        var trigger_name = document.getElementById("triggerSelected"+ruleId).innerText;
        var trigger_id = getIdByName(trigger_name);
        var target_name = document.getElementById("targetSelected"+ruleId).innerText;
        var target_id = getIdByName(target_name);

        if (trigger_id === undefined || target_id === undefined) {
            continue;
        }

        var triggerevt = document.getElementById("triggered"+ruleId).checked;
        var action = document.getElementById("activate"+ruleId).checked;
        var newRule = new ruleObject(trigger_id, triggerevt, target_id, action);
        rules.push(newRule)
    }
    var rulesPost = new XMLHttpRequest();
    rulesPost.onreadystatechange = function () {
        if (this.readyState === 4 && this.status === 20) {
            console.log("Sent rules successfully");
        }
    };
    rulesPost.open("POST", "/cgi/update_rules", true);
    var jsonstring = JSON.stringify(rules);
    console.log(jsonstring);
    rulesPost.send(jsonstring);
}
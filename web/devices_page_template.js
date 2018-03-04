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
function loadFile(path, type){
    if (type=="js"){
        var fileref=document.createElement('script');
        fileref.setAttribute("type","text/javascript");
        fileref.setAttribute("src", path);
    }
    else if (type=="css"){
        var fileref=document.createElement("link");
        fileref.setAttribute("rel", "stylesheet");
        fileref.setAttribute("type", "text/css");
        fileref.setAttribute("href", path);
    }
    document.getElementsByTagName("head")[0].appendChild(fileref);
}
var tDelay = 300;
setTimeout(function(){loadFile("devices_page_template.js", "js")} , 1*tDelay);
setTimeout(function(){loadFile("devices_page.js", "js")} , 2*tDelay);
setTimeout(function(){loadFile("timing_page_template.js", "js")} , 3*tDelay);
setTimeout(function(){loadFile("timing_page.js", "js")} , 4*tDelay);
setTimeout(function(){loadFile("homepage.js", "js")} , 5*tDelay);
setTimeout(function(){loadFile("timing_page.css", "css")} , 6*tDelay);
setTimeout(function(){loadFile("homepage.css", "css")} , 6*tDelay);
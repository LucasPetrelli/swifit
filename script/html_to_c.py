import os
import re
import sys

htmlHeader = (
    "\t\"HTTP/1.1 200 OK\\r\\n\"\n"
    "\t\"Content-type: text/html\\r\\n\\r\\n\"\n"
    "\t\"\"\n"
)

javascriptHeader = (
    "\t\"HTTP/1.1 200 OK\\r\\n\"\n"
    "\t\"Content-type: application/javascript\\r\\n\\r\\n\"\n"
    "\t\"\"\n"
)

cssHeader = (
    "\t\"HTTP/1.1 200 OK\\r\\n\"\n"
    "\t\"Content-type: text/css\\r\\n\\r\\n\"\n"
    "\t\"\"\n"

)

cssPattern = r"<link rel=\"stylesheet\" type=\"text/css\" href=\"(?P<file>.+\.css)\">"
jsPattern = r"<script src=\"(?P<file>.+\.js)\"></script>"

if __name__ == '__main__':
    target = 'C:\\repo\\swifit\\web\\master.html'
    outputPath = ''

    if len(sys.argv) > 1:
        target = sys.argv[1]
    if len(sys.argv) > 2:
        outputPath = sys.argv[2]

    targetFileName = target.split('\\')[-1]
    targetName, targetExtension = targetFileName.split('.')
    varName = "ac{}{}".format(targetName.capitalize(),targetExtension.capitalize())
    resourceName = "ac{}{}Id".format(targetName.capitalize(),targetExtension.capitalize())

    outputCFile = targetFileName + '.c'
    outputHFile = targetFileName + '.h'

    outputCPath = os.path.join(outputPath, outputCFile)
    outputHPath = os.path.join(outputPath, outputHFile)

    headerFileBuffer = "#include \"c_types.h\"\n" +\
                       "extern const char {}[];\n".format(varName) + \
                       "extern const char {}[];".format(resourceName)

    # with open(target) as inputFile:
    #     content = inputFile.read()

    # hasJs = re.search(jsPattern, content)
    # if (hasJs):
    #     jsPath = os.path.join(os.path.dirname(target), hasJs.group('file'))
    #     with open(jsPath) as jsFile:
    #         jsContent = jsFile.read()
    #     content = content.replace(hasJs.group(), '<script>\n{}\n</script>'.format(jsContent))
    #
    # hasCss = re.search(cssPattern, content)
    # if (hasCss):
    #     cssPath = os.path.join(os.path.dirname(target), hasCss.group('file'))
    #     with open(cssPath) as cssFile:
    #         cssContent = cssFile.read()
    #     content = content.replace(hasCss.group(), '<style>\n{}\n</style>'.format(cssContent))

    # target = target.replace(targetFileName.split('.')[0], targetFileName.split('.')[0]+'_')
    # with open(target, 'w') as intermediateFile:
    #     intermediateFile.write(content)

    with open(target) as inputFile:
        sourceFileBuffer = "#include \"{}\"\n\n".format(outputHFile)
        sourceFileBuffer += "const char {}[] = \"/{}\";\n".format(resourceName, targetFileName)
        sourceFileBuffer += "const char ICACHE_RODATA_ATTR STORE_ATTR {}[]".format(varName) + " = {\n"

        if targetExtension == 'html':
            sourceFileBuffer += htmlHeader
        elif targetExtension == 'css':
            sourceFileBuffer += cssHeader
        elif targetExtension == 'js':
            sourceFileBuffer += javascriptHeader

        for line in inputFile.readlines():
            sourceFileBuffer += '\t'
            sourceFileBuffer += '"{}"\n'.format(line.replace('\n', '').replace('\\', '\\\\').replace('"', r'\"'))
        sourceFileBuffer += '};'

    with open(outputCPath, 'w') as outC:
        outC.write(sourceFileBuffer)
    with open(outputHPath, 'w') as outH:
        outH.write(headerFileBuffer)
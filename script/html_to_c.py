import os
import sys

htmlHeader = (
    "\t\"HTTP/1.1 200 OK\\r\\n\"\n"
    "\t\"Content-type: text/html\\r\\n\\r\\n\"\n"
    "\t\"\"\n"
)

if __name__ == '__main__':
    target = r"C:\Users\Lucas\workspace\tcc_switch\tcc_eclipse_project\web\example.html"
    outputPath = ''

    if len(sys.argv) > 1:
        target = sys.argv[1]
    if len(sys.argv) > 2:
        outputPath = sys.argv[2]

    targetFileName = target.split('\\')[-1]
    varName = "ac{}Html".format(targetFileName.split('.')[0].capitalize())

    outputCFile = targetFileName + '.c'
    outputHFile = targetFileName + '.h'

    outputCPath = os.path.join(outputPath, outputCFile)
    outputHPath = os.path.join(outputPath, outputHFile)

    headerFileBuffer = "extern const char {}[];".format(varName)

    with open(target) as inputFile:
        sourceFileBuffer = "#include \"{}\"\n\n".format(outputHFile)
        sourceFileBuffer += "const char {}[]".format(varName) + " = {\n"
        sourceFileBuffer += htmlHeader
        for line in inputFile.readlines():
            sourceFileBuffer += '\t'
            sourceFileBuffer += '"{}"\n'.format(line.replace('"', r'\"').replace('\n', ''))
        sourceFileBuffer += '};'

    with open(outputCPath, 'w') as outC:
        outC.write(sourceFileBuffer)
    with open(outputHPath, 'w') as outH:
        outH.write(headerFileBuffer)
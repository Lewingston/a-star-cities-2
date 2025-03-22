
const path = require('path');
const fs = require('fs');

var rootDir = "../src/";

console.log("");

readTopDirs(rootDir);

function readTopDirs(dir) {

    var totalFileCount = 0;
    var totalLineCount = 0;

    var files = fs.readdirSync(dir);
    files.forEach(file => {

        if (fs.lstatSync(path.join(dir, file)).isDirectory()) {
            var result = readDir(path.join(dir, file));
            console.log(file);
            console.log("Files: " + result.fileCount);
            console.log("Lines: " + result.lineCount);
            console.log("");
            totalFileCount += result.fileCount;
            totalLineCount += result.lineCount;
        }
    });

    console.log("TOTAL:");
    console.log("Files: " + totalFileCount);
    console.log("Lines: " + totalLineCount);
}

function readDir(dir) {

    var fileCount = 0;
    var lineCount = 0;

    var files = fs.readdirSync(dir);
    files.forEach(file => {

        if (fs.lstatSync(path.join(dir, file)).isDirectory()) {
            var result = readDir(path.join(dir, file));
            fileCount += result.fileCount;
            lineCount += result.lineCount;
        } else {
            var fileName = path.join(dir, file);
            if (fileName.endsWith(".cpp") || fileName.endsWith(".h")) {
                fileCount += 1;
                lineCount += countLines(fileName);
            }
        }
    });

    return {fileCount: fileCount, lineCount: lineCount};
}

function countLines(file) {

    var lineCount = 1;

    try {
        var data = fs.readFileSync(file);
        for (var ii = 0; ii < data.length; ii++) {
            if (data[ii] == 10)
                lineCount++;
        }
    } catch (err) {
        console.error(err);
    }

    return lineCount;
}

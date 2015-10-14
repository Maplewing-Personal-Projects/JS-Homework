var fs = require('fs');

var input = fs.readFileSync("a.txt").toString();
input = input.substring(0, input.length-1);
input += "Maplewing";
fs.writeFileSync("b.txt", input);
var https = require("https");
/*
var options = {
	hostname: "zh.wikipedia.org",
	port: 80,
	path: "/wiki/Template:明朝歷史事件",
	method: "GET"
};

var req = https.request(options, function(res) {
	res.setEncoding('utf8');
	res.on('data', function(d) {
		console.log("=MAC=", d, "=MAC=");
	});
});
req.end();

req.on('error', function(e) {
  console.log("Got error: " + e.message);
});
*/

https.get("https://zh.wikipedia.org/")
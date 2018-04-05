var http = require('http');
var fs = require('fs');
var soap = require('soap');
var actionURL = 'http://www.axis.com/vapix/ws/action1/ActionService.wsdl';
var eventURL = 'http://www.axis.com/vapix/ws/event1/EventService.wsdl';
var body = '<x:Envelope xmlns:x="http://www.w3.org/2003/05/soap-envelope" xmlns:eve="http://www.axis.com/vapix/ws/action1">' +
'<x:Header/><x:Body><eve:GetActionRules/></x:Body></x:Envelope>'
var cam1URL = '192.168.100.24';
var cam2URL = 'http://192.168.100.26';
var username = 'root';
var password = 'password';
// var auth = 'Basic' + Buffer.from(username + ':' + password).toString('base64');


var postRequest = {
	host: cam1URL,
	path: "/vapix/services",
	method: "POST",
	auth: {
		user: "root",
		pass: "password"
	},
	headers: {
		'Content-Type': "text/xml; charset=utf-8",
		'SOAPAction': "http://www.axis.com/vapix/ws/action1/GetActionRules"
	}
}

var buffer = "";

var req = http.request( postRequest, function( res ) {
	console.log( res.statusCode );
	var buffer = "";
	res.on( "data", function( data ) { buffer = buffer + data; } );
	res.on( "end", function( data) { console.log( buffer ); } );
});

req.on('error', function(e) {
	console.log('problem with request: ' + e);
});

req.write(body);
req.end();

// soap.createClient(actionURL, /*{endpoint: 'http://192.168.100.24/vapix/services'},*/ function(err, client) {
// 	if(err) {
// 		console.log(err);
// 	}
// 	else {
// 		console.log(client.wsdl.descriptions);
// 	}
// });

// http.createServer(function (request, response) {
//     response.writeHead(200, {'Content-Type': 'text/html'});
//     fs.readFile('./index.html', null, function(error, data) {
//     	if (error) {
//     		response.writeHead(404);
//     		response.write('File not found!');
//     	} else {
//     		response.write(data);
//     	}
//     	response.end();
//     });
// }).listen(8080);

console.log('Server started');
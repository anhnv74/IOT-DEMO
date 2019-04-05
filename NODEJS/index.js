// build server, khai báo sử dụng socket io
var express = require("express");
var app = express();
app.use(express.static("public"));
app.set("view engine", "ejs");
app.set("views", "./views");

var server = require("http").Server(app);
var io = require("socket.io")(server);
//server.listen(3000);
var server_port = process.env.OPENSHIFT_NODEJS_PORT || 3000
var server_ip_address = process.env.OPENSHIFT_NODEJS_IP || '127.0.0.1'

server.listen(server_port, server_ip_address, function () {
  console.log( "Listening on " + server_ip_address + ", port " + server_port )
});


app.get("/", function(req, res)
	{
		res.render("HomePage");
	});

	var mqtt = require('mqtt');
	var options = {
	    port: 12064,
	    host: 'mqtt://m16.cloudmqtt.com',
	    clientId: 'mqttjs_' + Math.random().toString(16).substr(2, 8),
	    username: 'dsbwiuis',
	    password: 'glc9YQlmbcJS',
	    keepalive: 60,
	    reconnectPeriod: 1000,
	    protocolId: 'MQIsdp',
	    protocolVersion: 3,
	    clean: true,
	    encoding: 'utf8'
	};

	var client = mqtt.connect('mqtt://m16.cloudmqtt.com', options);

	client.on('connect', () => {
	  client.subscribe('topic1')
	})

	function ab2str(buf) {
	  return String.fromCharCode.apply(null, new Uint16Array(buf));
	}

  // tạo kết nối giữa client và server
io.on("connection", function(socket)
{
			//Nhân dữ liệu từ cliet
			socket.on("TURN_ON",function(data){
				//Gửi dữ liệu đến MQTT
					client.publish('topic2', 'ON')
			});
			//Nhân dữ liệu từ cliet
			socket.on("TURN_OFF",function(data){
					//Gửi dữ liệu đến MQTT
					client.publish('topic2', 'OFF')
			});
      //Nhận dữ liệu từ server MQTT
			client.on('message', (topic, message)=>{
				//console.log("Received '" + message + "' on '" + topic + "'");
				//Gửi dữ liệu cho client
				socket.emit("Server-sent-data", ab2str(message));
			})

});

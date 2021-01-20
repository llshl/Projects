from influxdb import InfluxDBClient
import json
import paho.mqtt.client as mqtt

server = "54.198.221.28"
influxClient = InfluxDBClient(server, 8086, 'root', 'root', 'mqttSensorDB')

def on_connect(client, userdata, flags, rc):
    print("Connected with RC : " + str(rc))
    print("Start Injecting!")
    client.subscribe("heum/2016146044/evt/#")

def on_message(client, userdata, msg):
    if (msg.payload.decode('utf-8') == ""):
        influxMsg = 0.0
    else:
        influxMsg = float(msg.payload.decode('utf-8'))

    json_body = [
	{
		"measurement": msg.topic,
		"tags": {
		    "host": "server01",
		    "region": "us-west"
		},
		"fields": {
		    "value" : influxMsg
		}
	}
    ]
    influxClient.write_points(json_body)


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(server, 1883, 60)

client.loop_forever()

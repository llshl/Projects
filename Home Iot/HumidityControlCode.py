import paho.mqtt.client as mqtt

server = "54.198.221.28"

def on_connect(client, userdata, flags, rc):
    print("Connected with RC: " + str(rc))
    client.subscribe("heum/2016146044/evt/humidity")

def on_message(client, userdata, msg):
    print(msg.topic+" "+msg.payload.decode('utf-8'))
    humidity = float(msg.payload.decode('utf-8'))

    if(humidity  < 40):
        client.publish("heum/2016146044/cmd/humidity ", "relayOn")
    else:
        client.publish("heum/2016146044/cmd/humidity ", "relayOff")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(server, 1883, 60)

client.loop_forever()

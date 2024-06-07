from .settings import MQTT_KEEPALIVE, MQTT_PORT, MQTT_SERVER
import paho.mqtt.client as mqtt


def on_connect(mqtt_client, userdata, flags, rc):
    if rc == 0:
        print('Connected successfully')
        mqtt_client.subscribe('IoT/water-tank-depth')
    else:
        print('Bad connection. Code:', rc)

def on_message(mqtt_client, userdata, msg):
    try:
        from IoT_hub.models import WaterTankDepth
        payload = float(msg.payload)
        WaterTankDepth.objects.create(depth=payload).save()

        print(f'Saved message on topic: {msg.topic} with payload: {msg.payload}')
    except ValueError:
        print(f'Payload {msg.payload} is not an integer')
        return

    



client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
# client.username_pw_set(settings.MQTT_USER, settings.MQTT_PASSWORD)

def connect():
    client.connect(
    host=MQTT_SERVER,
    port=MQTT_PORT,
    keepalive=MQTT_KEEPALIVE)
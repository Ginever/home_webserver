from .settings import MQTT_KEEPALIVE, MQTT_PORT, MQTT_SERVER
import paho.mqtt.client as mqtt


def on_connect(mqtt_client, userdata, flags, rc):
    """
        Logic for what to do when first connected to MQTT broker
    """
    if rc == 0:
        print('Connected successfully')

        # Subscribe to required topics
        mqtt_client.subscribe('IoT/water-tank-depth')
    else:
        print('Bad connection. Code:', rc)

def on_message(mqtt_client, userdata, msg):
    """
        Logic for what to do when a message is published on a subscribed channel
    """
    try: 
        if (msg.topic == 'IoT/water-tank-depth'):
            # If data received from water-tank-depth save to water tank depth data set if data is of valid form
            from IoT_hub.models import WaterTankDepth

            payload = float(msg.payload)  
            WaterTankDepth.objects.create(depth=payload).save()

            print(f'Saved message on topic: {msg.topic} with payload: {msg.payload}')
            return
        
        print(f"Unexpected message topic '{msg.topic}' with payload: {msg.payload}")

        
    except ValueError:
        print(f'Payload {msg.payload} is not an integer')
        return

    


# client config
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

#* For later use if needed
# client.username_pw_set(settings.MQTT_USER, settings.MQTT_PASSWORD)

def connect():
    """
        Connect client to MQTT broker
    """
    client.connect(
    host=MQTT_SERVER,
    port=MQTT_PORT,
    keepalive=MQTT_KEEPALIVE)
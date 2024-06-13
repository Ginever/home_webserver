from . import mqtt

# start MQTT services of server start up
mqtt.connect()
mqtt.client.loop_start()
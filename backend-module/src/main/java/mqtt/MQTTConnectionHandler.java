package mqtt;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.buffer.Buffer;
import io.vertx.core.json.JsonObject;
import io.vertx.mqtt.MqttClient;
import utils.Logger;

public class MQTTConnectionHandler extends AbstractVerticle {
    private static final String BROKER = "broker.hivemq.com";
    private static final int PORT = 1883;
    private static final String SENSOR_TOPIC = "Smart-air-purifier/sensor/data";
    private static final String FAN_SPEED_TOPIC = "Smart-air-purifier/fan/speed";
    private static final String MODE_COMMAND_TOPIC = "Smart-air-purifier/control/mode";
    private static final String FAN_COMMAND_TOPIC = "Smart-air-purifier/control/fan-speed";
    private static final String ALARM_TOPIC = "Smart-air-purifier/alarm";

    private MqttClient client;

    @Override
    public void start() {
        client = MqttClient.create(vertx);

        // Register one handler for all incoming publications; route by the actual topic.
        client.publishHandler(message -> {
            String topic = message.topicName();
            String payload = message.payload().toString();
            Logger.info("Received MQTT message on " + topic);

            try {
                if (SENSOR_TOPIC.equals(topic)) {
                    processSensorData(payload);
                } else if (FAN_SPEED_TOPIC.equals(topic)) {
                    JsonObject status = new JsonObject(payload);
                    String speed = status.getString("speed");
                    if (speed != null) {
                        vertx.eventBus().publish("device/fan/speed", speed);
                    }
                } else if (ALARM_TOPIC.equals(topic)) {
                    vertx.eventBus().publish("device/alarm", payload);
                }
            } catch (RuntimeException exception) {
                Logger.error("Ignoring invalid MQTT payload on " + topic + ": " + exception.getMessage());
            }
        });

        vertx.eventBus().consumer("http/command/fan-speed", message ->
                publishMessage(FAN_COMMAND_TOPIC, (String) message.body()));
        vertx.eventBus().consumer("http/command/mode", message ->
                publishMessage(MODE_COMMAND_TOPIC, (String) message.body()));

        client.connect(PORT, BROKER, result -> {
            if (result.succeeded()) {
                Logger.success("Connected to MQTT broker " + BROKER);
                subscribeToTopics();
            } else {
                Logger.error("Failed to connect to MQTT broker: " + result.cause().getMessage());
            }
        });
    }

    private void subscribeToTopics() {
        subscribeToTopic(SENSOR_TOPIC);
        subscribeToTopic(FAN_SPEED_TOPIC);
        subscribeToTopic(ALARM_TOPIC);
    }

    private void subscribeToTopic(String topic) {
        client.subscribe(topic, MqttQoS.AT_LEAST_ONCE.value(), result -> {
            if (result.succeeded()) {
                Logger.success("Subscribed to topic: " + topic);
            } else {
                Logger.error("Failed to subscribe to " + topic + ": " + result.cause().getMessage());
            }
        });
    }

    private void processSensorData(String payload) {
        JsonObject json = new JsonObject(payload);
        publishFloat(json, "temperature", "sensor/temperature");
        publishFloat(json, "humidity", "sensor/humidity");
        publishFloat(json, "COlevel", "sensor/COlevel");
        publishFloat(json, "CO2level", "sensor/CO2level");
    }

    private void publishFloat(JsonObject json, String key, String address) {
        Number value = json.getNumber(key);
        if (value != null) {
            vertx.eventBus().publish(address, value.floatValue());
        }
    }

    private void publishMessage(String topic, String payload) {
        if (client == null || !client.isConnected()) {
            Logger.error("Cannot publish to " + topic + ": MQTT client is not connected");
            return;
        }

        client.publish(topic, Buffer.buffer(payload), MqttQoS.AT_LEAST_ONCE, false, false, result -> {
            if (result.succeeded()) {
                Logger.success("Published command to " + topic);
            } else {
                Logger.error("Failed to publish to " + topic + ": " + result.cause().getMessage());
            }
        });
    }
}

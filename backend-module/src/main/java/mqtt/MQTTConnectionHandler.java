package mqtt;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.buffer.Buffer;
import io.vertx.core.json.JsonObject;
import io.vertx.mqtt.MqttClient;
import utils.Logger;

public class MQTTConnectionHandler extends AbstractVerticle {

    private MqttClient client;

    @Override
    public void start() {
        client = MqttClient.create(vertx);
        client.connect(1883, "broker.hivemq.com", s -> {
            if (s.succeeded()) {
                Logger.success("Connected to a server");
                subscribeToTopics();
            } else {
                Logger.error("Failed to connect to a server");
            }
        });
        vertx.eventBus().consumer("http/Smart-air-purifier/fan/speed", message -> {
            String speed = (String) message.body();
            publishMessage("Smart-air-purifier/fan/speed", speed);
        });
        vertx.eventBus().consumer("http/Smart-air-purifier/control/mode", message -> {
            String mode = (String) message.body();
            publishMessage("Smart-air-purifier/control/mode", mode);
        });
    }

    private void subscribeToTopics() {
        subscribeToTopic("Smart-air-purifier/sensor/data");
        subscribeToTopic("Smart-air-purifier/fan/speed");
        subscribeToTopic("Smart-air-purifier/control/mode");
    }

    private void subscribeToTopic(String topic) {
        client.publishHandler(s -> {
            String message = s.payload().toString();
            String receivedTopic = s.topicName();
            Logger.info("Received message: " + message + " on topic: " + receivedTopic);
            if (receivedTopic.equals("Smart-air-purifier/sensor/data")) {
                processSensorData(message);
            } else {
                vertx.eventBus().publish(topic, message);
            }
        }).subscribe(topic, MqttQoS.AT_LEAST_ONCE.value(), res -> {
            if (res.succeeded()) {
                Logger.success("Subscribed to topic: " + topic);
            } else {
                Logger.error("Failed to subscribe to topic: " + topic);
            }
        });
    }

    private void processSensorData(String message) {
        JsonObject json = new JsonObject(message);
        vertx.eventBus().publish("sensor/temperature", json.getFloat("temperature"));
        vertx.eventBus().publish("sensor/humidity", json.getFloat("humidity"));
        vertx.eventBus().publish("sensor/COlevel", json.getFloat("COlevel"));
        vertx.eventBus().publish("sensor/CO2level", json.getFloat("CO2level"));
    }

    private void publishMessage(String topic, String message) {
        if (message != null) {
            Buffer buffer = Buffer.buffer(message);
            client.publish(topic, buffer, MqttQoS.AT_LEAST_ONCE, false, false, res -> {
                if (res.succeeded()) {
                    Logger.success("Published message to topic " + topic);
                } else {
                    Logger.error("Failed to publish message to topic " + topic);
                }
            });
        }
    }
}

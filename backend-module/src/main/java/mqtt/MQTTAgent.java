package mqtt;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.AsyncResult;
import io.vertx.core.buffer.Buffer;
import io.vertx.mqtt.MqttClient;

public class MQTTAgent extends AbstractVerticle {

    private MqttClient client;

    @Override
    public void start() {
        client = MqttClient.create(vertx);
        client.connect(1883, "broker.hivemq.com", s -> {
            if (s.succeeded()) {
                System.out.println("Connected to a server");
                subscribeToTopic("Smart-air-purifier");
            } else {
                System.out.println("Failed to connect to a server");
            }
        });

        vertx.setPeriodic(5000, id -> {
            publishMessage("test/topic", "Hello from Vert.x");
        });

        // Add a publish handler to print received messages
        client.publishHandler(message -> {
            String topicName = message.topicName();
            String payload = message.payload().toString();
            System.out.println("Received message on topic " + topicName + ": " + payload);
        });
    }

    private void publishMessage(String topic, String message) {
        Buffer buffer = Buffer.buffer(message);
        client.publish("test/topic", buffer, MqttQoS.AT_LEAST_ONCE, false, false);
    }

    private void subscribeToTopic(String topic) {
        client.subscribe(topic, MqttQoS.AT_LEAST_ONCE.value(), this::handleSubscribe);
    }

    private void handleSubscribe(AsyncResult<Integer> res) {
        if (res.succeeded()) {
            System.out.println("Subscribed to topic");
        } else {
            System.out.println("Failed to subscribe to topic");
        }
    }

    private void handlePublish(AsyncResult<Integer> res) {
        if (res.succeeded()) {
            System.out.println("Message published");
        } else {
            System.out.println("Failed to publish message");
        }
    }
}
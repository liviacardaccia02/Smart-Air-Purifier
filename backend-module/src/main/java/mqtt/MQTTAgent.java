package mqtt;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.AsyncResult;
import io.vertx.core.buffer.Buffer;
import io.vertx.mqtt.MqttClient;
import logic.SharedMessage;
import utils.Logger;

public class MQTTAgent extends AbstractVerticle {

    private MqttClient client;
    private final SharedMessage<String> fanSpeed;

    public MQTTAgent(SharedMessage<String> fanSpeed) {
        this.fanSpeed = fanSpeed;
        this.fanSpeed.addSpeedChangeListener(message -> this.publishMessage("Smart-air-purifier", String.valueOf(message)));
    }

    @Override
    public void start() {
        client = MqttClient.create(vertx);
        client.connect(1883, "broker.hivemq.com", s -> {
            if (s.succeeded()) {
                Logger.success("Connected to a server");
                subscribeToTopic("Smart-air-purifier");
            } else {
                Logger.error("Failed to connect to a server");
            }
        });
    }

    private void publishMessage(String topic, String message) {
        if (message != null) {
            Buffer buffer = Buffer.buffer(message);
            client.publish(topic, buffer, MqttQoS.AT_LEAST_ONCE, false, false, this::handlePublishing);
            Logger.info("Published message to topic" + topic);
        }
    }

    private void subscribeToTopic(String topic) {
        client.publishHandler(s -> {
            String message = s.payload().toString();
            setSpeed(message);
        }).subscribe(topic, MqttQoS.AT_LEAST_ONCE.value(), this::handleSubscription);
    }

    private void handleSubscription(AsyncResult<Integer> res) {
        if (res.succeeded()) {
            Logger.success("Subscribed to topic");
        } else {
            Logger.error("Failed to subscribe to topic");
        }
    }

    private void handlePublishing(AsyncResult<Integer> handler) {
        if (handler.succeeded()) {
            Logger.success("Published message to topic");
        } else {
            Logger.error("Failed to publish message to topic");
        }
    }

    private void setSpeed(String message) {
        synchronized (fanSpeed) {
            fanSpeed.setMessage(message);
        }
    }
}
package org.example;

import com.hivemq.client.mqtt.MqttClient;
import com.hivemq.client.mqtt.mqtt3.Mqtt3AsyncClient;
import java.util.UUID;

public class Main {
    public static void main(String[] args) {
        Mqtt3AsyncClient client = MqttClient.builder()
                .identifier(UUID.randomUUID().toString())
                .serverHost("broker.hivemq.com")
                .serverPort(1883)
                .useMqttVersion3()
                .buildAsync();

        client.connect()
                .thenAccept(connAck -> System.out.println("Connected successfully"))
                .thenCompose(ack -> client.subscribeWith()
                        .topicFilter("Smart-air-purifier")
                        .callback(publish -> System.out.println("Received Message: " + new String(publish.getPayloadAsBytes())))
                        .send())
                .thenAccept(subAck -> System.out.println("Subscribed successfully"))
                .exceptionally(ex -> {ex.printStackTrace(); return null;});
    }
}
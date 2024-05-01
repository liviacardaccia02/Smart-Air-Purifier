package org.example;

import http.HTTPServer;
import io.vertx.core.Vertx;
import io.vertx.core.VertxOptions;
import logic.SharedMessage;
import mqtt.MQTTAgent;

import java.io.IOException;

public class Main {
    public static void main(String[] args) throws IOException {
        SharedMessage<Float> temperature = new SharedMessage<>(0.0f);
        SharedMessage<Float> humidity = new SharedMessage<>(0.0f);
        SharedMessage<Float> COlevel = new SharedMessage<>(0.0f);
        SharedMessage<Float> CO2level = new SharedMessage<>(0.0f);
        SharedMessage<Integer> fanSpeed = new SharedMessage<>(0);

        Vertx vertx = Vertx.vertx(new VertxOptions().setMaxEventLoopExecuteTime(Long.MAX_VALUE));
        MQTTAgent agent = new MQTTAgent();
        HTTPServer httpServer = new HTTPServer(temperature, humidity, COlevel, CO2level, fanSpeed);

        vertx.deployVerticle(agent, res -> {
            if (res.succeeded()) {
                System.out.println("MQTTAgent verticle deployed successfully");
            } else {
                System.out.println("Failed to deploy MQTTAgent verticle");
                res.cause().printStackTrace();
            }
        });

        httpServer.start();
    }
}
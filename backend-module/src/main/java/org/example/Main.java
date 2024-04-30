package org.example;

import io.vertx.core.Vertx;
import io.vertx.core.VertxOptions;
import mqtt.MQTTAgent;

public class Main {
    public static void main(String[] args) {
        Vertx vertx = Vertx.vertx(new VertxOptions().setMaxEventLoopExecuteTime(Long.MAX_VALUE));
        MQTTAgent agent = new MQTTAgent();

        vertx.deployVerticle(agent, res -> {
            if (res.succeeded()) {
                System.out.println("MQTTAgent verticle deployed successfully");
            } else {
                System.out.println("Failed to deploy MQTTAgent verticle");
                res.cause().printStackTrace();
            }
        });
    }
}
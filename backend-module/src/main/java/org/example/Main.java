package org.example;

import http.HTTPConnectionHandler;
import io.vertx.core.Vertx;
import mqtt.MQTTConnectionHandler;

public class Main {
    public static void main(String[] args) {
        Vertx vertx = Vertx.vertx();

        vertx.deployVerticle(new MQTTConnectionHandler(), res -> {
            if (res.succeeded()) {
                System.out.println("MQTTConnectionHandler verticle deployed successfully");
            } else {
                System.err.println("Failed to deploy MQTTConnectionHandler verticle");
                res.cause().printStackTrace();
            }
        });

        vertx.deployVerticle(new HTTPConnectionHandler(), res -> {
            if (res.succeeded()) {
                System.out.println("HTTPConnectionHandler verticle deployed successfully");
            } else {
                System.err.println("Failed to deploy HTTPConnectionHandler verticle");
                res.cause().printStackTrace();
            }
        });
    }
}

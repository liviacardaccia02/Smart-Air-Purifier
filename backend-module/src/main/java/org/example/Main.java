package org.example;

import http.HTTPConnectionHandler;
import io.vertx.core.Vertx;
import mqtt.MQTTConnectionHandler;
import simulator.ESPSimulator;
import java.io.IOException;

public class Main {
    public static void main(String[] args) throws IOException {
        Vertx vertx = Vertx.vertx();
        vertx.deployVerticle(new MQTTConnectionHandler(), res -> {
            if (res.succeeded()) {
                System.out.println("MQTTConnectionHandler verticle deployed successfully");
            } else {
                System.out.println("Failed to deploy MQTTConnectionHandler verticle");
                res.cause().printStackTrace();
            }
        });
        vertx.deployVerticle(new HTTPConnectionHandler(), res -> {
            if (res.succeeded()) {
                System.out.println("HTTPConnectionHandler verticle deployed successfully");
            } else {
                System.out.println("Failed to deploy HTTPConnectionHandler verticle");
                res.cause().printStackTrace();
            }
        });
        vertx.deployVerticle(new ESPSimulator(), res -> {
            if (res.succeeded()) {
                System.out.println("ESPSimulator verticle deployed successfully");
            } else {
                System.out.println("Failed to deploy ESPSimulator verticle");
                res.cause().printStackTrace();
            }
        });
    }
}
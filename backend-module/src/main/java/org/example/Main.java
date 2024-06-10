package org.example;

import http.HTTPServer;
import io.vertx.core.Vertx;
import mqtt.MQTTAgent;
import simulator.ESPSimulator;

import java.io.IOException;

public class Main {
    public static void main(String[] args) throws IOException {
        Vertx vertx = Vertx.vertx();
        vertx.deployVerticle(new MQTTAgent(), res -> {
            if (res.succeeded()) {
                System.out.println("MQTTAgent verticle deployed successfully");
            } else {
                System.out.println("Failed to deploy MQTTAgent verticle");
                res.cause().printStackTrace();
            }
        });
        vertx.deployVerticle(new HTTPServer(), res -> {
            if (res.succeeded()) {
                System.out.println("HTTPServer verticle deployed successfully");
            } else {
                System.out.println("Failed to deploy HTTPServer verticle");
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
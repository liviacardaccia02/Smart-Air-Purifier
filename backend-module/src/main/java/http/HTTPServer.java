package http;

import com.sun.net.httpserver.HttpExchange;
import logic.SharedMessage;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import com.sun.net.httpserver.HttpServer;
import utils.Pair;
import utils.Logger;

public class HTTPServer {
    private final SharedMessage<Float> temperature;
    private final SharedMessage<Float> humidity;
    private final SharedMessage<Float> COlevel;
    private final SharedMessage<Float> CO2level;
    private final SharedMessage<String> fanSpeed;
    private final ExecutorService executor = Executors.newCachedThreadPool();

    public HTTPServer(SharedMessage<Float> temperature,
                      SharedMessage<Float> humidity,
                      SharedMessage<Float> COlevel,
                      SharedMessage<Float> CO2level,
                      SharedMessage<String> fanSpeed) {
        this.temperature = temperature;
        this.humidity = humidity;
        this.COlevel = COlevel;
        this.CO2level = CO2level;
        this.fanSpeed = fanSpeed;
    }

    public void start() throws IOException {
        HttpServer server = HttpServer.create(new InetSocketAddress(8000), 0);
        server.createContext("/home", new AbstractHandler() {
            @Override
            void handleGetRequest(HttpExchange ex) throws IOException {
                Path path = Paths.get("../frontend-module/index.html");
                Path realPath = path.toRealPath();
                String response = Files.readString(realPath);
                sendResponse(ex, response);
            }

            @Override
            void handlePostRequest(HttpExchange ex) throws IOException {

            }
        });

        server.createContext("/style.css", new AbstractHandler() {
            @Override
            void handleGetRequest(HttpExchange ex) throws IOException {
                Path path = Paths.get("../frontend-module/style.css");
                Path realPath = path.toRealPath();
                String response = Files.readString(realPath);
                ex.getResponseHeaders().set("Content-Type", "text/css");
                sendResponse(ex, response);
            }

            @Override
            void handlePostRequest(HttpExchange ex) throws IOException {

            }
        });

        server.createContext("/data", new AbstractHandler() {
            @Override
            void handleGetRequest(HttpExchange ex) throws IOException {
                synchronized (temperature) {
                    synchronized (humidity) {
                        synchronized (COlevel) {
                            synchronized (CO2level) {
                                synchronized (fanSpeed) {
                                    String response = "{"
                                            + "\"Temperature\": " + temperature.getMessage() + ","
                                            + "\"Humidity\": " + humidity.getMessage() + ","
                                            + "\"CO level\": " + COlevel.getMessage() + ","
                                            + "\"CO2 level\": " + CO2level.getMessage() + ","
                                            + "\"Fan speed\": " + fanSpeed.getMessage()
                                            + "}";
                                    ex.getResponseHeaders().set("Content-Type", "application/json");
                                    sendResponse(ex, response);
                                }
                            }
                        }
                    }
                }
            }

            @Override
            void handlePostRequest(HttpExchange ex) throws IOException {
                String speed = new String(ex.getRequestBody().readAllBytes(), StandardCharsets.UTF_8);
                synchronized (fanSpeed) {
                    fanSpeed.setMessage(speed);
                    fanSpeed.notifyAll();
                    Logger.info("Speed: " + fanSpeed.getMessage());
                }
                sendResponse(ex, "POST received");
            }
        });

        server.createContext("/script.js", new AbstractHandler() {
            @Override
            void handleGetRequest(HttpExchange ex) throws IOException {
                Path path = Paths.get("../frontend-module/script.js");
                Path realPath = path.toRealPath();
                String response = Files.readString(realPath);
                ex.getResponseHeaders().set("Content-Type", "application/javascript");
                sendResponse(ex, response);
            }

            @Override
            void handlePostRequest(HttpExchange ex) throws IOException {

            }
        });

        server.setExecutor(executor);
        server.start();
        Logger.success("HTTP server started");
    }
}

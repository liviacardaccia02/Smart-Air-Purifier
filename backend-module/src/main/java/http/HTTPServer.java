package http;

import com.sun.net.httpserver.HttpExchange;
import logic.SharedMessage;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import com.sun.net.httpserver.HttpServer;

public class HTTPServer {
    private final SharedMessage<Float> temperature;
    private final SharedMessage<Float> humidity;
    private final SharedMessage<Float> COlevel;
    private final SharedMessage<Float> CO2level;
    private final SharedMessage<Integer> fanSpeed;
    private final ExecutorService executor = Executors.newCachedThreadPool();

    public HTTPServer(SharedMessage<Float> temperature,
                      SharedMessage<Float> humidity,
                      SharedMessage<Float> COlevel,
                      SharedMessage<Float> CO2level,
                      SharedMessage<Integer> fanSpeed) {
        this.temperature = temperature;
        this.humidity = humidity;
        this.COlevel = COlevel;
        this.CO2level = CO2level;
        this.fanSpeed = fanSpeed;
    }

    public void start() throws IOException {
        HttpServer server = HttpServer.create(new InetSocketAddress(8000), 0);
        server.createContext("/test", new AbstractHandler() {
            @Override
            void handleGetRequest(HttpExchange ex) throws IOException {
                synchronized (temperature) {
                    synchronized (humidity) {
                        synchronized (COlevel) {
                            synchronized (CO2level) {
                                synchronized (fanSpeed) {
                                    String response = "Temperature: " + temperature.getMessage() + "\n"
                                            + "Humidity: " + humidity.getMessage() + "\n"
                                            + "CO level: " + COlevel.getMessage() + "\n"
                                            + "CO2 level: " + CO2level.getMessage() + "\n"
                                            + "Fan speed: " + fanSpeed.getMessage();
                                    sendResponse(ex, response);
                                }
                            }
                        }
                    }
                }
            }

            @Override
            void handlePostRequest(HttpExchange ex) throws IOException {

            }
        });

        server.setExecutor(executor);
        server.start();
        System.out.println("HTTP server started");
    }
}

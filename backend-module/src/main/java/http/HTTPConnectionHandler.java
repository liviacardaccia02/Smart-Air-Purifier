package http;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.handler.StaticHandler;
import utils.Logger;

import java.util.Set;

public class HTTPConnectionHandler extends AbstractVerticle {
    private Float temperature;
    private Float humidity;
    private Float coLevel;
    private Float co2Level;
    private String fanSpeed = "UNKNOWN";
    private String mode = "AUTOMATIC";
    private String alarm = "false";

    private static final Set<String> FAN_SPEEDS = Set.of("OFF", "MEDIUM", "HIGH");
    private static final Set<String> MODES = Set.of("AUTOMATIC", "MANUAL");

    @Override
    public void start() {
        Router router = Router.router(vertx);
        router.get("/data").handler(ctx -> {
            JsonObject response = new JsonObject()
                    .put("temperature", temperature)
                    .put("humidity", humidity)
                    .put("COlevel", coLevel)
                    .put("CO2level", co2Level)
                    .put("fanSpeed", fanSpeed)
                    .put("mode", mode)
                    .put("alarm", alarm);
            ctx.response().putHeader("Content-Type", "application/json").end(response.encode());
        });

        router.post("/fan-speed").handler(ctx -> ctx.request().bodyHandler(body -> {
            String speed = body.toString().trim().toUpperCase();
            if (!FAN_SPEEDS.contains(speed)) {
                ctx.response().setStatusCode(400).putHeader("Content-Type", "application/json")
                        .end(new JsonObject().put("error", "speed must be OFF, MEDIUM, or HIGH").encode());
                return;
            }
            fanSpeed = speed;
            vertx.eventBus().publish("http/command/fan-speed", speed);
            ctx.response().putHeader("Content-Type", "application/json")
                    .end(new JsonObject().put("fanSpeed", fanSpeed).encode());
        }));

        router.post("/mode").handler(ctx -> ctx.request().bodyHandler(body -> {
            String requestedMode = body.toString().trim().toUpperCase();
            if (!MODES.contains(requestedMode)) {
                ctx.response().setStatusCode(400).putHeader("Content-Type", "application/json")
                        .end(new JsonObject().put("error", "mode must be AUTOMATIC or MANUAL").encode());
                return;
            }
            mode = requestedMode;
            vertx.eventBus().publish("http/command/mode", requestedMode);
            ctx.response().putHeader("Content-Type", "application/json")
                    .end(new JsonObject().put("mode", mode).encode());
        }));

        vertx.eventBus().consumer("sensor/temperature", message -> temperature = (Float) message.body());
        vertx.eventBus().consumer("sensor/humidity", message -> humidity = (Float) message.body());
        vertx.eventBus().consumer("sensor/COlevel", message -> coLevel = (Float) message.body());
        vertx.eventBus().consumer("sensor/CO2level", message -> co2Level = (Float) message.body());
        vertx.eventBus().consumer("device/fan/speed", message -> fanSpeed = (String) message.body());
        vertx.eventBus().consumer("device/alarm", message -> alarm = (String) message.body());

        router.route().handler(StaticHandler.create("../frontend-module"));
        vertx.createHttpServer().requestHandler(router).listen(8000, res -> {
            if (res.succeeded()) Logger.success("HTTP server started on port 8000");
            else Logger.error("Failed to start HTTP server: " + res.cause().getMessage());
        });
    }
}

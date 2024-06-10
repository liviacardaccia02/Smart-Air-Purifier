package http;

import io.vertx.core.AbstractVerticle;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.handler.StaticHandler;
import io.vertx.core.json.JsonObject;
import utils.Logger;

public class HTTPServer extends AbstractVerticle {

    private Float temperature;
    private Float humidity;
    private Float COlevel;
    private Float CO2level;
    private String fanSpeed;
    private String mode;

    @Override
    public void start() {
        Router router = Router.router(vertx);

        router.route("/*").handler(StaticHandler.create("../frontend-module"));

        router.get("/data").handler(ctx -> {
            JsonObject response = new JsonObject()
                    .put("temperature", temperature)
                    .put("humidity", humidity)
                    .put("COlevel", COlevel)
                    .put("CO2level", CO2level)
                    .put("fanSpeed", fanSpeed)
                    .put("mode", mode);
            ctx.response().putHeader("Content-Type", "application/json").end(response.encode());
        });

        router.post("/fan-speed").handler(ctx -> ctx.request().bodyHandler(body -> {
            String speed = body.toString();
            this.fanSpeed = speed;
            vertx.eventBus().publish("Smart-air-purifier/fan/speed", speed);
            ctx.response().end("Fan speed set to " + speed);
        }));

        router.post("/mode").handler(ctx -> ctx.request().bodyHandler(body -> {
            String mode = body.toString();
            this.mode = mode;
            vertx.eventBus().publish("Smart-air-purifier/control/mode", mode);
            ctx.response().end("Mode set to " + mode);
        }));

        vertx.eventBus().consumer("sensor/temperature", message -> temperature = (Float) message.body());

        vertx.eventBus().consumer("sensor/humidity", message -> humidity = (Float) message.body());

        vertx.eventBus().consumer("sensor/COlevel", message -> COlevel = (Float) message.body());

        vertx.eventBus().consumer("sensor/CO2level", message -> CO2level = (Float) message.body());

        vertx.eventBus().consumer("Smart-air-purifier/fan/speed", message -> fanSpeed = (String) message.body());

        vertx.createHttpServer().requestHandler(router).listen(8000, res -> {
            if (res.succeeded()) {
                Logger.success("HTTP server started on port 8000");
            } else {
                Logger.error("Failed to start HTTP server");
            }
        });
    }
}

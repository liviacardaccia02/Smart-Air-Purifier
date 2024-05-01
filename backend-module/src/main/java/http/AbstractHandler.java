package http;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;

import java.io.IOException;
import java.io.OutputStream;

public abstract class AbstractHandler implements HttpHandler {

    @Override
    public void handle(HttpExchange ex) throws IOException {
        if (ex.getRequestMethod().equals("OPTIONS")) {
            handleOptionsRequest(ex);
        } else if (ex.getRequestMethod().equals("GET")) {
            handleGetRequest(ex);
        } else if (ex.getRequestMethod().equals("POST")) {
            handlePostRequest(ex);
        } else {
            handleUnsupportedMethod(ex);
        }
    }

    protected void sendResponse(HttpExchange ex, String response) throws IOException {
        ex.getResponseHeaders().add("Access-Control-Allow-Origin", "*");
        ex.sendResponseHeaders(200, response.getBytes().length);
        try (OutputStream os = ex.getResponseBody()) {
            os.write(response.getBytes());
        }
    }

    private void handleOptionsRequest(HttpExchange ex) throws IOException {
        ex.getResponseHeaders().add("Access-Control-Allow-Origin", "*");
        ex.getResponseHeaders().add("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        ex.getResponseHeaders().add("Access-Control-Allow-Headers", "Content-Type,Authorization");
        ex.sendResponseHeaders(204, -1);
        ex.close();
    }

    private void handleUnsupportedMethod(HttpExchange ex) throws IOException {
        ex.getResponseHeaders().add("Access-Control-Allow-Origin", "*");
        ex.sendResponseHeaders(405, -1);
        ex.close();
    }

    abstract void handleGetRequest(HttpExchange ex) throws IOException;

    abstract void handlePostRequest(HttpExchange ex) throws IOException;
}
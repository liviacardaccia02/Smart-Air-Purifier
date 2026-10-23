# Smart Air Purifier

An academic IoT prototype for environmental sensing and fan control, built around an ESP32, MQTT, a Java/Vert.x backend, and a browser dashboard.

## Architecture

- **Embedded controller:** ESP32 firmware reads the DHT11 and MQ-series sensors, applies the prototype's control logic, and publishes telemetry over MQTT.
- **Backend:** Java with Vert.x exposes the dashboard and HTTP API, translates HTTP control requests into MQTT commands, and forwards telemetry to the dashboard.
- **Dashboard:** HTML, CSS, and JavaScript display current readings and submit fan/mode controls.

## Repository layout

- `embedded-controller-module/` — PlatformIO firmware and device libraries
- `backend-module/` — Java/Gradle backend
- `frontend-module/` — static dashboard served by the backend

## Backend setup

Use a Java version supported by the Gradle project, then from `backend-module` run:

```bash
./gradlew run
```

The HTTP server listens on port `8000`. The dashboard is served by the backend, so open `http://localhost:8000` rather than opening `index.html` directly.

The dashboard API is:

- `GET /data` — latest known sensor readings, fan speed, mode, and alarm state
- `POST /fan-speed` — plain-text body `OFF`, `MEDIUM`, or `HIGH`
- `POST /mode` — plain-text body `AUTOMATIC` or `MANUAL`

Control requests are validated. Device commands use the MQTT topics `Smart-air-purifier/control/fan-speed` and `Smart-air-purifier/control/mode`; telemetry is published separately.

## Embedded setup and credentials

The firmware deliberately contains no Wi-Fi or MQTT credentials. Configure the local PlatformIO build with the preprocessor macros `WIFI_SSID` and `WIFI_PASSWORD`. If your MQTT broker requires authentication, also set `MQTT_USERNAME` and `MQTT_PASSWORD`. Quote/escape string values as required by your shell and PlatformIO environment. Do not commit credentials to source control.

The current firmware uses the public HiveMQ broker over unencrypted MQTT on port 1883. This is suitable only for a demonstration; public topics may be observed or interfered with. Use a private broker with authentication and TLS before any deployment beyond a local prototype.

## Known limitations

- The MQ-7 CO reading is still a placeholder in the sensor implementation and is not a calibrated measurement.
- Sensor calibration, gas concentration conversion, fan-control thresholds, and behavior must be validated with the actual hardware. This repository change does not claim physical testing.
- The energy chart is illustrative sample data, not measured energy consumption.
- The project is an academic proof of concept, not a safety-rated air-quality monitor or production purifier.

## Technologies

C++, ESP32, PlatformIO, MQTT, Java, Vert.x, Gradle, HTML, CSS, JavaScript.

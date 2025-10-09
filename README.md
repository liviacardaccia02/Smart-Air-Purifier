# Smart Air Purifier🌬️

Smart Air Purifier is an IoT-based prototype developed as part of a thesis project focused on intelligent air quality monitoring and control.
The system is designed to measure air quality, analyze the data, and control an air purification unit automatically based on environmental conditions.

Built as a proof of concept, the project integrates an ESP32-based embedded controller, a lightweight backend, and a web dashboard for visualization and interaction.
The communication between devices and services relies on MQTT and HTTP protocols, showcasing how distributed IoT systems can manage real-time data and control.

# System Architecture🧩

The system is organized into three main components:

**embedded-controller-module** → Runs on ESP32; collects air quality data (e.g., particulate matter, gas concentration) and sends it to the backend. It also manages fan control logic.<br/>
**backend-module** → Lightweight HTTP server that receives, processes, and stores sensor data. Handles communication with the frontend and embedded device.<br/>
**frontend-module** → Web dashboard for visualizing air quality levels and device status, and for controlling the purifier.<br/>

# Technologies Used⚙️

| Layer                 | Technology                      | Description                                            |
| --------------------- | ------------------------------- | ------------------------------------------------------ |
| Hardware              | **ESP32**                       | Handles sensor acquisition and fan control logic       |
| Communication         | **MQTT**, **HTTP**              | Used for real-time data transmission between modules   |
| Backend               | **Custom HTTP Server (Java)**   | Simple server for data collection and API handling     |
| Frontend              | **HTML, CSS, JavaScript**       | Displays air quality metrics and control interface     |
| Programming Languages | **Java**, **C++**, **HTML/CSS** | Used across backend, embedded, and frontend components |

# Features🚀

- Continuous air quality monitoring via ESP32 sensors
- MQTT-based data exchange between embedded device and backend
- HTTP API for storing and retrieving readings
- Web dashboard for visualizing air quality data
- Fan control logic implemented (partial — requires code revision for full functionality)
- Modular design separating embedded, backend, and frontend concerns

# Project Status🧠

This project is a prototype built for academic and demonstrative purposes as part of a thesis.
It is not intended for real-world or production use.<br/>
✅ Air quality sensing and data transmission working<br/>
✅ Dashboard displays data correctly<br/>
⚙️ Fan control logic partially implemented (requires further testing)<br/>
❌ No production deployment planned<br/>

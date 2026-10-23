async function requestJson(path, options = {}) {
    const response = await fetch(path, {
        ...options,
        headers: {
            "Content-Type": "text/plain",
            ...(options.headers || {}),
        },
    });

    const payload = await response.json();
    if (!response.ok) {
        throw new Error(payload.error || `Request failed: ${response.status}`);
    }
    return payload;
}

function displayValue(id, value, fallback = "—") {
    document.getElementById(id).textContent =
        value === null || value === undefined || value === "" ? fallback : value;
}

async function fetchData() {
    try {
        const data = await requestJson("/data");
        displayValue("temperature-status", data.temperature);
        displayValue("humidity-status", data.humidity);
        displayValue("co-status", data.COlevel, "—");
        displayValue("co2-status", data.CO2level, "—");
        displayValue("fan-speed", data.fanSpeed, "UNKNOWN");
        displayValue("mode", data.mode, "UNKNOWN");
        displayValue("alert", data.alarm === "true" ? "ALERT" : "NONE");

        const modeToggle = document.getElementById("modeToggle");
        modeToggle.checked = data.mode === "MANUAL";
    } catch (error) {
        console.error("Unable to fetch purifier data:", error);
    }
}

function getCurrentDate() {
    return new Intl.DateTimeFormat(undefined, {
        weekday: "long",
        month: "long",
        day: "numeric",
    }).format(new Date());
}

function drawChart() {
    const canvas = document.getElementById("energy-chart");
    if (!canvas || typeof Chart === "undefined") return;

    new Chart(canvas.getContext("2d"), {
        type: "line",
        data: {
            labels: ["Day 1", "Day 2", "Day 3", "Day 4", "Day 5"],
            datasets: [{
                label: "Energy Consumption (illustrative)",
                data: [19, 22, 18, 25, 20],
                fill: false,
                borderWidth: 2,
            }],
        },
        options: {
            responsive: true,
            scales: { y: { beginAtZero: true } },
        },
    });
}

async function changeFanSpeed(element) {
    const speed = element.dataset.speed;
    if (!speed) return;

    try {
        const result = await requestJson("/fan-speed", {
            method: "POST",
            body: speed,
        });
        displayValue("fan-speed", result.fanSpeed);
    } catch (error) {
        console.error("Unable to change fan speed:", error);
        alert(`Could not change fan speed: ${error.message}`);
    }
}

window.addEventListener("DOMContentLoaded", () => {
    displayValue("current-date", getCurrentDate());
    drawChart();
    fetchData();
    window.setInterval(fetchData, 3000);

    const modeToggle = document.getElementById("modeToggle");
    modeToggle.addEventListener("change", async () => {
        const mode = modeToggle.checked ? "MANUAL" : "AUTOMATIC";
        try {
            const result = await requestJson("/mode", {
                method: "POST",
                body: mode,
            });
            displayValue("mode", result.mode);
        } catch (error) {
            modeToggle.checked = !modeToggle.checked;
            console.error("Unable to change operating mode:", error);
            alert(`Could not change mode: ${error.message}`);
        }
    });

    document.querySelectorAll("[data-speed]").forEach(element => {
        element.addEventListener("click", event => {
            event.preventDefault();
            changeFanSpeed(element);
        });
    });
});

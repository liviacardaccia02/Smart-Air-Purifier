function fetchData() {
    fetch("http://localhost:8000/data", {
        method: "GET",
        mode: "cors",
        headers: {
            "Content-Type": "application/json",
        },
    })
        .then(response => response.json())
        .then(data => {
            console.log(data);
            document.getElementById('temperature-status').textContent = `${data["temperature"]}`;
            document.getElementById('humidity-status').textContent = `${data["humidity"]}`;
            document.getElementById('co-status').textContent = `${data["COlevel"]} ppm`;
            document.getElementById('co2-status').textContent = `${data["CO2level"]} ppm`;
        })
        .catch(error => {
            console.error("Error fetching data:", error);
        });

    fetch("http://localhost:8000/fan-speed", {
        method: "GET",
        mode: "cors",
        headers: {
            "Content-Type": "application/json",
        },
    })
        .then(response => response.json())
        .then(data => {
            console.log(data);
            // Process the fetched data here
            document.getElementById('fan-speed').textContent = `${data["fanSpeed"]}`;
        })
        .catch(error => {
            console.error("Error fetching data:", error);
        });

    fetch("http://localhost:8000/mode", {
        method: "GET",
        mode: "cors",
        headers: {
            "Content-Type": "application/json",
        },
    })
        .then(response => response.json())
        .then(data => {
            console.log(data);
            // Process the fetched data here
            document.getElementById('mode').textContent = `${data["mode"]}`;
        })
        .catch(error => {
            console.error("Error fetching data:", error);
        });
}

function getCurrentDate() {
    const months = ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'];
    const days = ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'];

    const currentDate = new Date();
    const dayOfWeek = days[currentDate.getDay()];
    const month = months[currentDate.getMonth()];
    const dayOfMonth = currentDate.getDate();
    const suffix = getNumberSuffix(dayOfMonth);

    return `${dayOfWeek}, ${month} ${dayOfMonth}${suffix}`;
}

function getNumberSuffix(dayOfMonth) {
    if (dayOfMonth >= 11 && dayOfMonth <= 13) {
        return 'th';
    }
    switch (dayOfMonth % 10) {
        case 1: return 'st';
        case 2: return 'nd';
        case 3: return 'rd';
        default: return 'th';
    }
}

function drawChart() {
    const ctx = document.getElementById("energy-chart").getContext("2d");
    const energyChart = new Chart(ctx, {
        type: "line",
        data: {
            labels: ["Day 1", "Day 2", "Day 3", "Day 4", "Day 5"],
            datasets: [{
                label: "Energy Consumption",
                data: [19, 22, 18, 25, 20],
                fill: false,
                borderColor: "rgba(75, 192, 192, 1)",
                borderWidth: 2,
            }],
        },
        options: {
            scales: {
                y: {
                    beginAtZero: true,
                },
            },
        },
    });
}

function changeFanSpeed(elem) {
    const selectedOption = elem.textContent;
    document.getElementById("fan-speed").textContent = selectedOption;
}

window.onload = () => {
    fetchData();
    document.getElementById("current-date").textContent = getCurrentDate();
    drawChart();

    const modeToggle = document.getElementById("modeToggle");
    modeToggle.addEventListener("change", function () {
        if (modeToggle.checked) {
            document.getElementById("mode").textContent = "MANUAL";
        } else {
            document.getElementById("mode").textContent = "AUTOMATIC";
        }
    });
}

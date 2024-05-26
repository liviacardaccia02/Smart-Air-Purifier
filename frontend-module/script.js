// function fetchAirQualityData(latitude, longitude) {
//     const baseUrl = "http://localhost:8000/google-api";
//     const query = `locations=${latitude},${longitude}&key=YOUR_API_KEY`;
//     const url = `${baseUrl}?${query}`;

//     return fetch(url)
//         .then(response => response.json())
//         .then(data => {
//             const snapshot = data.snapshots[0]; // Assuming single location data
//             return snapshot;
//         })
//         .catch(error => {
//             console.error("Error fetching air quality data:", error);
//         });
// }

// function updateAirQualityDisplay(snapshot) {
//     const aqiIndexElement = document.getElementById("aqi-index");
//     aqiIndexElement.textContent = `${snapshot.aqi}`;

//     const dominantPollutantElement = document.getElementById("dominant-pollutant");
//     dominantPollutantElement.textContent = `${snapshot.dominant_pollutant}`;
// }

// function initMap() {
//     navigator.geolocation.getCurrentPosition(
//         function (position) {
//             const latitude = position.coords.latitude;
//             const longitude = position.coords.longitude;
//             fetchAirQualityData(latitude, longitude)
//                 .then((snapshot) => updateAirQualityDisplay(snapshot));
//         },
//         function (error) {
//             console.error("Error getting geolocation:", error);
//             // Handle location access errors (e.g., permission denied)
//             // You can provide a default location or allow manual input
//         }
//     );
//}

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
            // Process the fetched data here
            speed = data["Fan speed"];
            document.getElementById('co-status').textContent = `${data["CO level"]} ppm`;
            document.getElementById('co2-status').textContent = `${data["CO2 level"]} ppm`;
            document.getElementById('temperature-status').textContent = `${data.Temperature}`;
            document.getElementById('humidity-status').textContent = `${data.Humidity}`;
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
    //initMap();

    const modeToggle = document.getElementById("modeToggle");
    modeToggle.addEventListener("change", function () {
        if (modeToggle.checked) {
            document.getElementById("mode").textContent = "MANUAL";
        } else {
            document.getElementById("mode").textContent = "AUTOMATIC";
        }
    });
}

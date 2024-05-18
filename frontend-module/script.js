function fetchData() {
    fetch("http://localhost:8000/data", {
        method: "GET",
        mode: "cors",
        headers: {
            "Content-Type": "text/plain",
        },
    })
        .then(response => response.json())
        .then(data => {
            console.log(data);
            // Process the fetched data here
            speed = data.speed;
            document.getElementById('co-status').textContent = `${data.co} ppm`;
            document.getElementById('co2-status').textContent = `${data.co2} ppm`;
        })
    // .catch(error => {
    //     // Handle any errors that occur during the fetch request
    //     console.error(error);
    // });
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
    src = "https://cdn.jsdelivr.net/npm/chart.js";

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

function handleDropdownClick(option) {
    document.getElementById("dropdown-content").textContent = option;
}



window.onload = () => {

    document.getElementById("dropdown-off").addEventListener("click", () => {
        handleDropdownClick("Off");
    });

    document.getElementById("dropdown-medium").addEventListener("click", () => {
        handleDropdownClick("Medium");
    });

    document.getElementById("dropdown-high").addEventListener("click", () => {
        handleDropdownClick("High");
    });

    fetchData();
    document.getElementById("current-date").textContent = getCurrentDate();
    drawChart();
}

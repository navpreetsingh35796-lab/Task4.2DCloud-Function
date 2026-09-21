// Replace with the IP address printed on your Arduino Serial Monitor
const ARDUINO_IP = "http://192.168.32.133";

const statusBox = document.getElementById("status-box");

function sendToggle(roomName) {
  statusBox.textContent = `Sending command to ${roomName}...`;

  const requestUrl = `${ARDUINO_IP}/toggle?room=${encodeURIComponent(roomName)}`;

  // mode: 'no-cors' allows local file requests to complete even across strict network boundaries
  fetch(requestUrl, { mode: "no-cors" })
    .then(() => {
      statusBox.textContent = `Success: Toggled ${roomName}`;
    })
    .catch((err) => {
      statusBox.textContent = `Network Error: Could not reach Arduino`;
      console.error(err);
    });
}

// Event Listeners for buttons
document.getElementById("btn-living").addEventListener("click", () => {
  sendToggle("living room");
});

document.getElementById("btn-bath").addEventListener("click", () => {
  sendToggle("bathroom");
});

document.getElementById("btn-closet").addEventListener("click", () => {
  sendToggle("closet");
});
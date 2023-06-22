let PORT = 8080;
let socket = new WebSocket(`ws://${window.location.hostname}:${PORT}`, "KomiX");

socket.onopen = (event) => {
    console.log("WebSocket connection established.");
};

socket.onmessage = (event) => {
    console.log("Received message:", event.data);
};

socket.onclose = (event) => {
    console.log("WebSocket connection closed.");
};

socket.onerror = (error) =>  {
    console.log("WebSocket error:", error);
};
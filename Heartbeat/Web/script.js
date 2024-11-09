let heartRate = 60; // Giá trị nhịp tim ban đầu

function updateHeartRate() {
    // Giả lập nhịp tim tăng giảm ngẫu nhiên
    heartRate = Math.floor(Math.random() * (100 - 50 + 1)) + 50; // Từ 50 đến 100 BPM
    document.getElementById('heartbeat').textContent = heartRate;
}

// Cập nhật nhịp tim mỗi giây
setInterval(updateHeartRate, 1000);

import java.util.HashMap;
import java.util.Map;

public class HealthDataProcessor {
    // Phương thức để in dữ liệu sức khỏe
    public static void printHealthData(Map<String, String> healthData) {
        System.out.println("Thông tin sức khỏe:");
        for (Map.Entry<String, String> entry : healthData.entrySet()) {
            System.out.println(entry.getKey() + ": " + entry.getValue());
        }
    }

    public static void main(String[] args) {
        // Tạo một HashMap để chứa dữ liệu sức khỏe
        Map<String, String> healthData = new HashMap<>();
        healthData.put("Nhịp tim", "75 bpm");
        healthData.put("SPO2", "98%");
        healthData.put("Trạng thái", "Bình thường");

        // In dữ liệu sức khỏe
        printHealthData(healthData);
    }
}

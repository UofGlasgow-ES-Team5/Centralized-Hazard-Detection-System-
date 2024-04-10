import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.HashMap;

public class SensorGUI extends JFrame {
    private JLabel titleLabel, co2Label, humidityLabel, temperatureLabel, co2LimitLabel, humidityLimitLabel, temperatureLimitLabel;
    private JTextField co2TextField, humidityTextField, temperatureTextField, co2LimitTextField, humidityLimitTextField, temperatureLimitTextField;
    private JButton editLimitsButton;
    private HashMap<String, Integer> sensorData;
    private HashMap<String, Integer> sensorLimits;

    public SensorGUI() {
        super("Hazard Detection System");
        setSize(500, 200);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        JPanel centerPanel = new JPanel(new GridLayout(4, 4));

       // titleLabel = new JLabel("<html><b>Hazard Detection System</b></html>");
        //titleLabel.setHorizontalAlignment(SwingConstants.CENTER);

        co2Label = new JLabel("CO2 Level:");
        humidityLabel = new JLabel("Humidity:");
        temperatureLabel = new JLabel("Temperature:");
        co2LimitLabel = new JLabel("CO2 Limit:");
        humidityLimitLabel = new JLabel("Humidity Limit:");
        temperatureLimitLabel = new JLabel("Temperature Limit:");

        co2TextField = new JTextField();
        humidityTextField = new JTextField();
        temperatureTextField = new JTextField();
        co2TextField.setEditable(false);
        humidityTextField.setEditable(false);
        temperatureTextField.setEditable(false);

        co2LimitTextField = new JTextField();
        humidityLimitTextField = new JTextField();
        temperatureLimitTextField = new JTextField();
        co2LimitTextField.setEditable(false);
        humidityLimitTextField.setEditable(false);
        temperatureLimitTextField.setEditable(false);

        editLimitsButton = new JButton("Edit Limits");
        editLimitsButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                editLimits();
            }
        });
        
        // Set preferred size for the JButton
        editLimitsButton.setPreferredSize(new Dimension(30, 30));

        centerPanel.add(co2Label);
        centerPanel.add(co2TextField);
        centerPanel.add(co2LimitLabel);
        centerPanel.add(co2LimitTextField);
        centerPanel.add(humidityLabel);
        centerPanel.add(humidityTextField);
        centerPanel.add(humidityLimitLabel);
        centerPanel.add(humidityLimitTextField);
        centerPanel.add(temperatureLabel);
        centerPanel.add(temperatureTextField);
        centerPanel.add(temperatureLimitLabel);
        centerPanel.add(temperatureLimitTextField);
        centerPanel.add(editLimitsButton);

       // add(titleLabel, BorderLayout.NORTH);
        add(centerPanel, BorderLayout.CENTER);
       // add(editLimitsButton, BorderLayout.CENTER);

        sensorData = new HashMap<>();
        sensorLimits = new HashMap<>();
        loadSensorData();
        loadSensorLimits();
        updateSensorData();
        updateSensorLimits();
    }

    private void loadSensorData() {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("sensor_data.txt"));
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(":");
                sensorData.put(parts[0], Integer.parseInt(parts[1]));
            }
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void loadSensorLimits() {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("sensor_limits.txt"));
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(":");
                sensorLimits.put(parts[0], Integer.parseInt(parts[1]));
            }
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void updateSensorData() {
        int co2Value = sensorData.getOrDefault("CO2", 0);
        int humidityValue = sensorData.getOrDefault("Humidity", 0);
        int temperatureValue = sensorData.getOrDefault("Temperature", 0);

        co2TextField.setText(String.valueOf(co2Value));
        humidityTextField.setText(String.valueOf(humidityValue));
        temperatureTextField.setText(String.valueOf(temperatureValue));

        checkAndSetTextColor(co2TextField, co2Value, "CO2");
        checkAndSetTextColor(humidityTextField, humidityValue, "Humidity");
        checkAndSetTextColor(temperatureTextField, temperatureValue, "Temperature");
    }

    private void checkAndSetTextColor(JTextField textField, int value, String sensorType) {
        if (sensorLimits.containsKey(sensorType)) {
            int limit = sensorLimits.get(sensorType);
            if (value > limit) {
                textField.setForeground(Color.RED);
            } else {
                textField.setForeground(Color.BLACK);
            }
        }
    }

    private void updateSensorLimits() {
        co2LimitTextField.setText(String.valueOf(sensorLimits.getOrDefault("CO2", 0)));
        humidityLimitTextField.setText(String.valueOf(sensorLimits.getOrDefault("Humidity", 0)));
        temperatureLimitTextField.setText(String.valueOf(sensorLimits.getOrDefault("Temperature", 0)));
    }

    private void editLimits() {
        try {
            BufferedWriter writer = new BufferedWriter(new FileWriter("sensor_limits.txt"));
            for (String sensorType : sensorLimits.keySet()) {
                String limit = JOptionPane.showInputDialog("Enter limit for " + sensorType + ":");
                sensorLimits.put(sensorType, Integer.parseInt(limit));
                writer.write(sensorType + ":" + limit + "\n");
            }
            writer.close();
            updateSensorLimits();
        } catch (IOException e) {
            e.printStackTrace();
        }
        JOptionPane.showMessageDialog(this, "Limits updated successfully");
    }

    public static void main(String[] args) {
        SensorGUI gui = new SensorGUI();
        gui.setVisible(true);
    }
}

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class HDFrame extends JFrame implements ActionListener {
	
	private JButton EditButton,SaveButton;
    private HashMap<String, Integer> sensorData;
    private HashMap<String, Integer> sensorLimits;
    private JTextField co2T,tempT,humT,co2TL,tempTL,humTL,locT,msgT;
    private int co2, humidity, temperature;
    
	HDFrame(){
//---------------------BUTTONS--------------------------//		
		EditButton = new JButton();
		SaveButton = new JButton();
		EditButton.setBounds(33, 200, 70,20);
		EditButton.addActionListener(this);
		EditButton.setText("Edit");
		EditButton.setFocusable(false);
		SaveButton.addActionListener(this);
		SaveButton.setBounds(33,250, 70, 20);
		SaveButton.addActionListener(this);
		SaveButton.setText("Save");
		SaveButton.setFocusable(false);
		
//---------------------TextField------------------------//
		
		co2T = new JTextField();
		tempT = new JTextField();
		humT = new JTextField();
		locT = new JTextField();
		msgT = new JTextField();
		co2TL = new JTextField();
		tempTL = new JTextField();
		humTL = new JTextField();
		
		co2T.setBounds(90, 50, 80,20);
		co2T.setFocusable(false);
		co2T.setEditable(false);
		co2T.setBackground(Color.GRAY);
		co2T.setForeground(Color.BLACK);
		co2T.setFont(new Font("Arial",Font.BOLD,15)); 
		
		
		tempT.setBounds(90, 100, 80,20);
		tempT.setFocusable(false);
		tempT.setBackground(Color.GRAY);
		tempT.setEditable(false);
		tempT.setForeground(Color.BLACK);
		tempT.setFont(new Font("Arial",Font.BOLD,15)); 


		
		humT.setBounds(90, 150, 80,20);
		humT.setFocusable(false);
		humT.setBackground(Color.GRAY);
		humT.setEditable(false);
		humT.setForeground(Color.BLACK);
		humT.setFont(new Font("Arial",Font.BOLD,15)); 


		
		co2TL.setBounds(30, 50, 80,20);
		co2TL.setEditable(false);
		co2TL.setBackground(Color.GRAY);
		co2TL.setForeground(Color.BLACK);
		co2TL.setFont(new Font("Arial",Font.BOLD,15)); 


		
		tempTL.setBounds(30, 100, 80,20);
		tempTL.setEditable(false);
		tempTL.setBackground(Color.GRAY);
		tempTL.setForeground(Color.BLACK);
		tempTL.setFont(new Font("Arial",Font.BOLD,15)); 


		
		humTL.setBounds(30, 150, 80,20);
		humTL.setEditable(false);
		humTL.setBackground(Color.GRAY);
		humTL.setForeground(Color.BLACK);
		humTL.setFont(new Font("Arial",Font.BOLD,15)); 


		
		locT.setBounds(10, 50, 139,200);
		locT.setFocusable(false);
		locT.setEditable(false);
		locT.setBackground(Color.DARK_GRAY);
		locT.setFont(new Font("Arial",Font.BOLD,15)); 
		locT.setForeground(Color.RED);

		
		msgT.setBounds(10, 23, 480,70);
		msgT.setFocusable(false);
		msgT.setEditable(true);
		msgT.setBackground(Color.DARK_GRAY);
		msgT.setForeground(Color.BLACK);
		msgT.setFont(new Font("Arial",Font.BOLD,15)); 
	
		
//----------------------Labels-------------------------//
		JLabel Title_label = new JLabel();
		JLabel CO2_label = new JLabel();
		JLabel Temp_label = new JLabel();
		JLabel Humidity_label = new JLabel();
		JLabel Limits_label = new JLabel();
		JLabel Loc_label = new JLabel();
		JLabel Message_label = new JLabel();
		
		//Title//
		Title_label.setText("Hazard Detection System");
		Title_label.setHorizontalAlignment(JLabel.CENTER);
		Title_label.setVerticalAlignment(JLabel.TOP);
		Title_label.setForeground(Color.CYAN);
		Title_label.setFont(new Font("Arial",Font.BOLD,20)); 
		//Title_Label.setBorder(BorderFactory.createLineBorder(Color.GREEN,3));
		Title_label.setBounds(100, 5, 300,50);
		
		//CO2//
		CO2_label.setText("CO2               :");
		CO2_label.setHorizontalAlignment(JLabel.LEFT);
		CO2_label.setVerticalAlignment(JLabel.TOP);
		CO2_label.setForeground(Color.BLACK);
		CO2_label.setFont(new Font("Open Sans Bold",Font.BOLD,13)); 
		//CO2_label.setBorder(BorderFactory.createLineBorder(Color.GREEN,3));
		CO2_label.setBounds(10, 50, 80,50);
		
		//Temperature//
		Temp_label.setText("TEMP.           :");
		Temp_label.setHorizontalAlignment(JLabel.LEFT);
		Temp_label.setVerticalAlignment(JLabel.TOP);
		Temp_label.setForeground(Color.BLACK);
		Temp_label.setFont(new Font("Open Sans Bold",Font.BOLD,13)); 
		//Temp_label.setBorder(BorderFactory.createLineBorder(Color.GREEN,3));
		Temp_label.setBounds(10, 100, 80,50);
		
		//Humidity//
		Humidity_label.setText("Humidity     :");
		Humidity_label.setHorizontalAlignment(JLabel.LEFT);
		Humidity_label.setVerticalAlignment(JLabel.TOP);
		Humidity_label.setForeground(Color.BLACK);
		Humidity_label.setFont(new Font("Open Sans Bold",Font.BOLD,13)); 
		//Title_Label.setBorder(BorderFactory.createLineBorder(Color.GREEN,3));
		Humidity_label.setBounds(10, 150, 80,50);
		
		//Location//
		Loc_label.setText("Location :");
		Loc_label.setHorizontalAlignment(JLabel.CENTER);
		Loc_label.setVerticalAlignment(JLabel.TOP);
		Loc_label.setForeground(Color.BLACK);
		Loc_label.setFont(new Font("Arial",Font.BOLD,15)); 
		//Title_Label.setBorder(BorderFactory.createLineBorder(Color.GREEN,3));
		Loc_label.setBounds(30, 5, 100,50);
		
		Limits_label.setText("Threshold :");
		Limits_label.setHorizontalAlignment(JLabel.CENTER);
		Limits_label.setVerticalAlignment(JLabel.TOP);
		Limits_label.setForeground(Color.BLACK);
		Limits_label.setFont(new Font("Arial",Font.BOLD,15)); 
		//Title_Label.setBorder(BorderFactory.createLineBorder(Color.GREEN,3));
		Limits_label.setBounds(20, 5, 100,50);
		
		//Message//
		Message_label.setText("Messages :");
		Message_label.setHorizontalAlignment(JLabel.CENTER);
		Message_label.setVerticalAlignment(JLabel.TOP);
		Message_label.setForeground(Color.CYAN);
		Message_label.setFont(new Font("Arial",Font.BOLD,15)); 
		//Title_Label.setBorder(BorderFactory.createLineBorder(Color.GREEN,3));
		Message_label.setBounds(0,5, 150,20);
		
		
		
//---------------------Panels-------------------------//		
		JPanel P_Title = new JPanel();
		JPanel P_sensorDataDisplay = new JPanel();
		JPanel P_sensorLimits = new JPanel();
		JPanel P_Location = new JPanel();
		JPanel P_Message = new JPanel();
		
		P_Title.setBackground(Color.BLACK);
		P_sensorDataDisplay.setBackground(Color.GRAY);
		P_sensorLimits.setBackground(Color.GRAY);
		P_Location.setBackground(Color.GRAY);
		P_Message.setBackground(Color.BLACK);
		
//		P_Title.setBorder(BorderFactory.createLineBorder(Color.ORANGE,2));
		P_sensorDataDisplay.setBorder(BorderFactory.createLineBorder(Color.ORANGE,2));
		P_sensorLimits.setBorder(BorderFactory.createLineBorder(Color.ORANGE,2));
		P_Location.setBorder(BorderFactory.createLineBorder(Color.ORANGE,2));
//		P_Message.setBorder(BorderFactory.createLineBorder(Color.ORANGE,2));

		P_Title.setPreferredSize(new Dimension(520,50));
		P_sensorDataDisplay.setPreferredSize(new Dimension(200,370));	
		P_sensorLimits.setPreferredSize(new Dimension(160,370));	
		P_Location.setPreferredSize(new Dimension(160,370));	
		P_Message.setPreferredSize(new Dimension(520,100));	
		
		P_Title.add(Title_label);
		P_sensorDataDisplay.setLayout(null);
		P_sensorDataDisplay.add(CO2_label);	
		P_sensorDataDisplay.add(Temp_label);
		P_sensorDataDisplay.add(Humidity_label);
		P_sensorDataDisplay.add(co2T);
		P_sensorDataDisplay.add(tempT);
		P_sensorDataDisplay.add(humT);
		
		P_sensorLimits.setLayout(null);
		P_sensorLimits.add(co2TL);
		P_sensorLimits.add(tempTL);
		P_sensorLimits.add(humTL);
		P_sensorLimits.add(EditButton);
		P_sensorLimits.add(SaveButton);
		P_sensorLimits.add(Limits_label);
		
		P_Location.setLayout(null);
		P_Location.add(Loc_label);
		P_Location.add(locT);
		
		P_Message.setLayout(null);
		P_Message.add(Message_label);
		P_Message.add(msgT);
				

//-----------------------Frame----------------------------//	
		this.setTitle("Hazard Detection System");
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setResizable(false);
		this.setSize(520,520);
		this.setLayout(new BorderLayout());
		this.setVisible(true);
		this.getContentPane().setBackground(Color.black);
		this.add(P_Title,BorderLayout.NORTH);
		this.add(P_sensorDataDisplay,BorderLayout.WEST);
		this.add(P_sensorLimits,BorderLayout.CENTER);
		this.add(P_Location,BorderLayout.EAST);
		this.add(P_Message,BorderLayout.SOUTH);
		
//-------------------------------------------------------//
		
        sensorData = new HashMap<>();
        sensorLimits = new HashMap<>();
        
        
        loadSensorData();
        loadSensorLimits();
        updateSensorData();
        updateSensorLimits();
        updateMessage();
        
        SensorDataUpdater sensorDataUpdater = new SensorDataUpdater(this);
        Thread sensorDataThread = new Thread(sensorDataUpdater);
        sensorDataThread.start();

        
	}
	
	
//---------------------------------------------------------------Functions-------------------------------------------------------------------//
	public HashMap<String, Integer> getSensorData() {
          return sensorData;
      }
      	
    protected void loadSensorData() {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("../sensor_data.txt"));
            
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(":");
                if (parts.length == 2) { // Ensure the line has correct format
                    sensorData.put(parts[0], Integer.parseInt(parts[1]));
                }
            }
            reader.close();
            
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    private void loadSensorLimits() {
        try {
            BufferedReader reader = new BufferedReader(new FileReader("../sensor_limits.txt"));
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(":");
                if (parts.length == 2) { // Ensure the line has correct format
                    sensorLimits.put(parts[0], Integer.parseInt(parts[1]));
                }
            }
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    protected String loadMessage(String filename) throws IOException {
        StringBuilder content = new StringBuilder();
        BufferedReader reader = new BufferedReader(new FileReader(filename));
        String line;

        while ((line = reader.readLine()) != null) {
            content.append(line).append("\n");
        }

        reader.close();
        return content.toString();
    }
    
    protected void updateMessage() {
    	
    	int co2 = sensorData.get("CO2");
        int temperature = sensorData.get("Temperature");
        int humidity = sensorData.get("Humidity");
        
        int co2L = sensorLimits.get("CO2");
        int temperatureL = sensorLimits.get("Temperature");
        int humidityL = sensorLimits.get("Humidity");
        
		if(co2 > co2L ) {
			msgT.setBackground(Color.YELLOW);
			msgT.setText("**Warning:\nCO2 level has exceeded permissible limit.**");
			try {
				String location = loadMessage("../location.txt");
				locT.setText(location);
			   
		   } catch (IOException e) {
			   e.printStackTrace();
			   locT.setText("Error: Unable to read file.");
		   }
		}
		else if (temperature > temperatureL ) {
			msgT.setBackground(Color.YELLOW);
			msgT.setText("**Warning:\nTemperature level has exceeded permissible limit.**");
			try {
				String location = loadMessage("../location.txt");
				locT.setText(location);
			   
		   } catch (IOException e) {
			   e.printStackTrace();
			   locT.setText("Error: Unable to read file.");
		   }
		}
		else if(humidity > humidityL ) {
			msgT.setBackground(Color.YELLOW);
			msgT.setText("**Warning:\nHumidity level has exceeded permissible limit.**");
			try {
				String location = loadMessage("../location.txt");
				locT.setText(location);
			   
		   } catch (IOException e) {
			   e.printStackTrace();
			   locT.setText("Error: Unable to read file.");
		   }
		}
		else {
			msgT.setBackground(Color.DARK_GRAY);
			try {
	            String content = loadMessage("../message.txt");
	            msgT.setText(content);
	        } catch (IOException e) {
	            e.printStackTrace();
	            msgT.setText("Error: Unable to read file.");
	        }
		}
    	
    	setVisible(true);
    }
    
    protected void updateSensorData() {
        int co2Value = sensorData.getOrDefault("CO2", 0);
        int temperatureValue = sensorData.getOrDefault("Temperature", 0);
        int humidityValue = sensorData.getOrDefault("Humidity", 0);
        

        co2T.setText(String.valueOf(co2Value));
        tempT.setText(String.valueOf(temperatureValue));
        humT.setText(String.valueOf(humidityValue));
        
        checkAndSetTextColor(co2T, co2Value, "CO2");
        checkAndSetTextColor(humT, humidityValue, "Humidity");
        checkAndSetTextColor(tempT, temperatureValue, "Temperature");
        

    }
    
    private void checkAndSetTextColor(JTextField textField, int value, String sensorType) {
        if (sensorLimits.containsKey(sensorType)) {
            int limit = sensorLimits.get(sensorType);
            if (value > limit) {
                textField.setBackground(Color.RED);
            } else {
                textField.setBackground(Color.GRAY);
            }
        }
    }
    
    private void updateSensorLimits() {
    	co2TL.setText(String.valueOf(sensorLimits.getOrDefault("CO2", 0)));
    	tempTL.setText(String.valueOf(sensorLimits.getOrDefault("Temperature", 0)));
    	humTL.setText(String.valueOf(sensorLimits.getOrDefault("Humidity", 0)));
    }

	@Override
	public void actionPerformed(ActionEvent e) {
		if(e.getSource()==EditButton) {
			co2TL.setEditable(true);
			humTL.setEditable(true);
			tempTL.setEditable(true);
			co2TL.setBackground(Color.WHITE);
			humTL.setBackground(Color.WHITE);
			tempTL.setBackground(Color.WHITE);
			
		
		}
		
		if(e.getSource() == SaveButton) {
			co2TL.setEditable(false);
			tempTL.setEditable(false);
			humTL.setEditable(false);
			co2TL.setBackground(Color.GRAY);
			tempTL.setBackground(Color.GRAY);
			humTL.setBackground(Color.GRAY);
			
			
			String CO2 = co2TL.getText();
			String temp = tempTL.getText();
			String hum = humTL.getText();
			
             try {
                  co2 = Integer.parseInt(CO2);
                  temperature = Integer.parseInt(temp);
                  humidity = Integer.parseInt(hum);   
             } catch (NumberFormatException ex) {
                 JOptionPane.showMessageDialog(null, "Please enter a valid number", "Error", JOptionPane.ERROR_MESSAGE);
             }
             sensorLimits.put("CO2", co2);
             sensorLimits.put("Temperature", temperature);
             sensorLimits.put("Humidity", humidity);
             
             try {
                 BufferedWriter writer = new BufferedWriter(new FileWriter("../sensor_limits.txt"));
                 writer.write("CO2:" + co2 + "\n");
                 writer.write("Temperature:" + temperature + "\n");
                 writer.write("Humidity:" + humidity + "\n");
                 writer.close();
                 updateSensorLimits();
             } catch (IOException ez) {
                 ez.printStackTrace();
             }         
		}
		
	}
}

	
 
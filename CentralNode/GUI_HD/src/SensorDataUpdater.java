
public class SensorDataUpdater implements Runnable {
    private HDFrame hdFrame;

    public SensorDataUpdater(HDFrame hdFrame) {
        this.hdFrame = hdFrame;
    }


	@Override
    public void run() {
        while (true) {
            try {
                Thread.sleep(1000); // Check every 1 seconds 
                hdFrame.loadSensorData();
                hdFrame.updateSensorData();
				hdFrame.updateMessage();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}


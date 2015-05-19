package pl.edu.agh.eaiib.uclimate.tabpager;

import pl.edu.agh.eaiib.uclimate.UClimateData;

/**
 * Created by Jakub Szczepankiewicz on 2015-05-15.
 */
public interface Updateable {
    /**
     * Updates fragment's views with current UClimateData
     * @param data - current data received from server
     */
    public void updateData(UClimateData data);
}

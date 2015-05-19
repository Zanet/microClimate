package pl.edu.agh.eaiib.uclimate.tabpager;

import android.content.Context;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentStatePagerAdapter;

import java.util.ArrayList;
import java.util.List;

import pl.edu.agh.eaiib.uclimate.UClimateData;

/**
 * Adapter for tabs
 * Created by Jakub Szczepankiewicz on 2015-05-15.
 */
public class TabPagerAdapter extends FragmentStatePagerAdapter {
    UClimateData data;

    public TabPagerAdapter(FragmentManager fm, UClimateData data, Context context) {
        super(fm);
        this.data = data;
    }

    @Override
    public Fragment getItem(int position) {
        switch(position){
            case 0:
                return new TemperatureTab(data.getTemperature_press());
            case 1:
                return new HumidityTab(data.getHumidity());
            case 2:
                return new PressureTab(data.getPressure());
            default:
                return null;
        }
    }

    @Override
    public int getCount() {
        return 3;
    }

    /**
     * If object is instance of Updateable(like PressureTab, TemperatureTab etc.)
     * then invoke updateData(UClimateData) method
     * @param object
     * @return
     */
    @Override
    public int getItemPosition(Object object){
        if (object instanceof Updateable) {
            ((Updateable) object).updateData(data);
        }
        return super.getItemPosition(object);
    }

    public void setData(UClimateData data) {
        this.data = data;
    }

}

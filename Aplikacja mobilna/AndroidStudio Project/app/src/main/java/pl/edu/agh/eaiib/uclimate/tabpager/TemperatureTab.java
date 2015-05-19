package pl.edu.agh.eaiib.uclimate.tabpager;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.ImageView;

import java.text.DecimalFormat;

import pl.edu.agh.eaiib.uclimate.R;
import pl.edu.agh.eaiib.uclimate.UClimateData;

/**
 * Fragment tab for temperature
 * Created by Jakub Szczepankiewicz on 2015-05-15.
 */
public class TemperatureTab extends MyFragment implements Updateable{
    TemperatureTab(){
        super();
    }
    TemperatureTab(float value){
        super(value);
    }
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        View temperature = inflater.inflate(R.layout.temperature_frag, container, false);
        title = Float.toString(value)+" deg.";
        ((TextView) temperature.findViewById(R.id.textView)).setText(title);
        return temperature;
    }

    @Override
    public void updateData(UClimateData data) {
        value = data.getTemperature_press();
        ((TextView) getView().findViewById(R.id.textView)).setText(Float.toString(value)+" deg.");
    }
}

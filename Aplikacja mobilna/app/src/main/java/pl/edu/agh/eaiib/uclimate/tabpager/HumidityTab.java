package pl.edu.agh.eaiib.uclimate.tabpager;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.text.DecimalFormat;

import pl.edu.agh.eaiib.uclimate.R;
import pl.edu.agh.eaiib.uclimate.UClimateData;

/**
 * Fragment tab for humidity
 * Created by Jakub Szczepankiewicz on 2015-05-15.
 */
public class HumidityTab extends MyFragment implements Updateable{

    HumidityTab(){
        super();
    }
    HumidityTab(float value){
        super(value);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        View humidity = inflater.inflate(R.layout.humidity_frag, container, false);
        title = Float.toString(value)+"%";
        ((TextView) humidity.findViewById(R.id.textView)).setText(title);
        return humidity;
    }

    @Override
    public void updateData(UClimateData data) {
        value = data.getHumidity();
        ((TextView) getView().findViewById(R.id.textView)).setText(Float.toString(value)+"%");
    }
}

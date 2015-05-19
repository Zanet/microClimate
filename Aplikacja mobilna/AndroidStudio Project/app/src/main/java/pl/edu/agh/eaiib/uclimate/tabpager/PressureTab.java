package pl.edu.agh.eaiib.uclimate.tabpager;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import java.text.DecimalFormat;

import pl.edu.agh.eaiib.uclimate.R;
import pl.edu.agh.eaiib.uclimate.UClimateData;

/**
 * Fragment tab for pressure
 * Created by Jakub Szczepankiewicz on 2015-05-15.
 */
public class PressureTab extends MyFragment implements Updateable{

    PressureTab(){
        super();
    }
    PressureTab(float value){
        super(value);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        View pressure = inflater.inflate(R.layout.pressure_frag, container, false);
        title = Float.toString(value)+" hPa";
        ((TextView) pressure.findViewById(R.id.textView)).setText(title);
        return pressure;
    }

    @Override
    public void updateData(UClimateData data) {
        value = data.getPressure();
        ((TextView) getView().findViewById(R.id.textView)).setText(Float.toString(value)+" hPa");
        if(value > 1040.0) {
            ImageView im = (ImageView) getView().findViewById(R.id.image);
            im.setImageResource(R.drawable.sun);
        } else if(value >= 1000.0 && value <=1040.0){
            ImageView im = (ImageView) getView().findViewById(R.id.image);
            im.setImageResource(R.drawable.sunncloud);
        } else if(value < 1000.0) {
            ImageView im = (ImageView) getView().findViewById(R.id.image);
            im.setImageResource(R.drawable.cloud);
        }
    }
}

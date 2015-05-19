package pl.edu.agh.eaiib.uclimate.tabpager;

import android.support.v4.app.Fragment;

/**
 * Created by Jakub Sczepankiewicz on 2015-05-15.
 */
public class MyFragment extends Fragment {
    protected float value=0;
    protected String title="";

    MyFragment(){
        super();
    }
    MyFragment(float value){
        this.value = value;
    }

    public void setValue(float value){
        this.value = value;
    }
    public float getValue(){
        return value;
    }

    public String getTitle(){
        return title;
    }
}

package pl.edu.agh.eaiib.uclimate.tabpager;

/**
 * Created by Siimp on 2015-05-15.
 */
public interface UpdateListener {
    public void onScrollChanged(int index);

    public int getCurrentScrollIndex();
}

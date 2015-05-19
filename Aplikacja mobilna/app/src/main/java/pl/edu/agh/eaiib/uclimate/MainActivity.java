package pl.edu.agh.eaiib.uclimate;

import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.AsyncTask;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.support.v4.app.FragmentTransaction;
import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.view.Menu;
import android.view.MenuItem;

import pl.edu.agh.eaiib.uclimate.connector.*;
import pl.edu.agh.eaiib.uclimate.tabpager.*;

/**
 * Main activity class
 */
public class MainActivity extends AppCompatActivity{

    /**
     * Hosts the section contents.
     */
    ViewPager mViewPager;

    TabPagerAdapter tabAdapter;
    ActionBar actionBar;

    /**
     * Holds current data from server
     */
    UClimateData data = new UClimateData();

    Connector server;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tabAdapter = new TabPagerAdapter(getSupportFragmentManager(),data, this);

        mViewPager = (ViewPager) findViewById(R.id.pager);
        mViewPager.setOnPageChangeListener(new ViewPager.SimpleOnPageChangeListener() {
            @Override
            public void onPageSelected(int position) {
                super.onPageSelected(position);
                actionBar.setSelectedNavigationItem(position);
            }
        });
        mViewPager.setAdapter(tabAdapter);
        addActionBarTabs();

}

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    /**
     * AsyncTask for connecting with server
     */
    private class AsyncOperation extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... arg0) {
            server = new Connector(data);
            server.getData();
            return null;
        }

        @Override
        protected void onPreExecute() {
        }

        @Override
        protected void onProgressUpdate(Void... values) {
        }
    }

    /**
     * Options menu to update data
     * @param item
     * @return
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();

        if (id == R.id.update) {
            //data.testing();
            new AsyncOperation().execute();
            tabAdapter.setData(data);
            tabAdapter.notifyDataSetChanged();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    /**
     * Adds tabs to action bar
     */
    private void addActionBarTabs() {
        actionBar = getSupportActionBar();
        String[] tabs = { "Temperature", "Humidity", "Pressure" };
        for (String tabTitle : tabs) {
            ActionBar.Tab tab = actionBar.newTab().setText(tabTitle)
                    .setTabListener(new ActionBar.TabListener() {
                        @Override
                        public void onTabSelected(ActionBar.Tab tab, FragmentTransaction ft) {
                            mViewPager.setCurrentItem(tab.getPosition());
                        }

                        @Override
                        public void onTabUnselected(ActionBar.Tab tab, FragmentTransaction ft) {
                        }

                        @Override
                        public void onTabReselected(ActionBar.Tab tab, FragmentTransaction ft) {
                        }
                    }
            );
            actionBar.addTab(tab);
        }
        actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);
    }
}

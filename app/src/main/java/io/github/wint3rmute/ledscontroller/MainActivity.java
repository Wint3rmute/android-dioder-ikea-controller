package io.github.wint3rmute.ledscontroller;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

import com.azeesoft.lib.colorpicker.ColorPickerDialog;
import com.crystal.crystalrangeseekbar.interfaces.OnSeekbarChangeListener;
import com.crystal.crystalrangeseekbar.widgets.CrystalSeekbar;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;


public class MainActivity extends AppCompatActivity {

    private ColorPickerDialog colorPickerDialog;
    private View currentColor;
    private CrystalSeekbar seekBarR, seekBarG, seekBarB;
    private OutputStream outputStream;
    private InputStream inStream; //never used but who knows what might happen yeah?
    private BluetoothDevice arduino;
    private int r, g, b;

    public void write(String s) throws IOException {
        Log.e("sending", "'" + (int) s.charAt(0) + "'");
        outputStream.write(s.getBytes());
    }


    private void init() {
        BluetoothAdapter blueAdapter = BluetoothAdapter.getDefaultAdapter();
        if (blueAdapter != null) {
            if (blueAdapter.isEnabled()) {
                Set<BluetoothDevice> bondedDevices = blueAdapter.getBondedDevices();

                if (bondedDevices.size() > 0) {
                    Object[] devices = bondedDevices.toArray();
                    //BluetoothDevice device = (BluetoothDevice) devices[position];

                    Log.e("device", "listing devices:");
                    boolean foundArduino = false;

                    for (int i = 0; i < devices.length; i++) {
                        BluetoothDevice device = (BluetoothDevice) devices[i];
                        //Log.e("device", device.getName());

                        if (device.getName().toLowerCase().equals("lofi_robot")) {
                            //Log.e("connecting", "found the device!");
                            foundArduino = true;
                            arduino = (BluetoothDevice) devices[i];
                            break;
                        }
                    }

                    if(!foundArduino)
                    {
                        Snackbar.make(findViewById(android.R.id.content), "Device not paired!", Snackbar.LENGTH_LONG)
                                .setAction("Action", null).show();
                        return;
                    }

                    try {
                        BluetoothSocket socket = arduino.createRfcommSocketToServiceRecord(arduino.getUuids()[0].getUuid());
                        socket.connect();
                        outputStream = socket.getOutputStream();
                        inStream = socket.getInputStream();
                    } catch (IOException e) {
                        Snackbar.make(findViewById(android.R.id.content), "Creating rfcommsocket failed!", Snackbar.LENGTH_LONG)
                                .setAction("Action", null).show();
                    }
                    Snackbar.make(findViewById(android.R.id.content), "Connected succesfully!", Snackbar.LENGTH_LONG)
                            .setAction("Action", null).show();

                } else
                    Snackbar.make(findViewById(android.R.id.content), "Device not paired! Password: 1234", Snackbar.LENGTH_LONG)
                            .setAction("Action", null).show();
            } else {
                Snackbar.make(findViewById(android.R.id.content), "Bluetooth is turned off!", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        }else{
            Snackbar.make(findViewById(android.R.id.content), "No bluetooth on this device!", Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show();
        }
    }


    private void refreshColor(int color) throws IOException {
        Log.e("refreshing color", "NOW");
        r = (int) (((float) ((color >> 16) & 0xFF) / 4));
        g = (int) (((float) ((color >> 8) & 0xFF) / 4));
        b = (int) (((float) ((color >> 0) & 0xFF) / 4));

        Log.e("new color", r + " " + g + " " + b);


        try {
            updateColor();
        } catch (Exception e) {

        }
    }

    private void refreshColor(int code, int value) throws IOException {
        switch (code) {
            case 1:
                r = value;
                break;
            case 2:
                g = value;
                break;
            case 3:
                b = value;
                break;
        }
        try {
            updateColor();
        } catch (Exception e) {

        }
    }

    private void updateColor() throws IOException {
        currentColor.setBackgroundColor(Color.rgb(r*4,g*4, b*4));
        write(((char) (r) + ""));
        write((char) (g+64)+"");
        write((char) (b+128)+"");

    }

    private void initUI() {
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        colorPickerDialog = ColorPickerDialog.createColorPickerDialog(this, ColorPickerDialog.DARK_THEME);
        seekBarR = (CrystalSeekbar) findViewById(R.id.seekBarR);
        seekBarG = (CrystalSeekbar) findViewById(R.id.seekBarG);
        seekBarB = (CrystalSeekbar) findViewById(R.id.seekBarB);

        currentColor = (View) findViewById(R.id.currentColor);
    }

    private void setListeners() {
        colorPickerDialog.setOnColorPickedListener(new ColorPickerDialog.OnColorPickedListener() {
            @Override
            public void onColorPicked(int color, String hexVal) {
                Log.e("color picker", color + "");
                try {
                    refreshColor(color);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

        seekBarR.setOnSeekbarChangeListener(new OnSeekbarChangeListener() {
            @Override
            public void valueChanged(Number value) {
                try {
                    refreshColor(1, value.intValue());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

        seekBarG.setOnSeekbarChangeListener(new OnSeekbarChangeListener() {
            @Override
            public void valueChanged(Number value) {
                try {
                    refreshColor(2, value.intValue());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

        seekBarB.setOnSeekbarChangeListener(new OnSeekbarChangeListener() {
            @Override
            public void valueChanged(Number value) {
                try {
                    refreshColor(3, value.intValue());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initUI();
        setListeners();


        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                Log.e("connecting", "in progress");
                Snackbar.make(view, "Connecting...", Snackbar.LENGTH_LONG)
                        .setAction("Action",  null).show();

                try {
                    AsyncTask btInit = new AsyncTask() {
                        @Override
                        protected Object doInBackground(Object[] params) {
                            init();
                            return null;
                        }
                    }.execute();


                } catch (Exception e) {
                    Snackbar.make(view, "Failed to connect!", Snackbar.LENGTH_LONG)
                            .setAction("Action", null).show();
                }
            }
        });
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    public void pickColor(View view) {
        colorPickerDialog.show();
    }
}

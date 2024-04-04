package io.github.hexstr.UnityFPSUnlocker;

import android.content.SharedPreferences;
import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

public class PerAppSettingsActivity extends AppCompatActivity {
    public static SharedPreferences prefs_ = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.settings_activity);
        if (savedInstanceState == null) {
            getSupportFragmentManager()
                    .beginTransaction()
                    .replace(R.id.settings, new PerAppSettingsFragment())
                    .commit();
        }
        prefs_ = Prefs.getSharedPrefs(this);
    }
}
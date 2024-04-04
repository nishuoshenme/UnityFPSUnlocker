package io.github.hexstr.UnityFPSUnlocker;

import android.content.SharedPreferences;
import android.os.Bundle;

import androidx.preference.PreferenceFragmentCompat;

import java.util.Map;

public class PerAppSettingsFragment extends PreferenceFragmentCompat {
    SharedPreferences.OnSharedPreferenceChangeListener listener =
            (sharedPreferences, key) -> {
                String packageName = sharedPreferences.getString("package_name", "");
                if (packageName.isEmpty()) {
                    return;
                }

                Map<String, ?> allEntries = sharedPreferences.getAll();
                SharedPreferences.Editor editor = PerAppSettingsActivity.prefs_.edit();

                for (Map.Entry<String, ?> entry : allEntries.entrySet()) {
                    String entry_key = entry.getKey();
                    Object entry_value = entry.getValue();

                    String prefixed_key = packageName + "_" + entry_key;

                    if (entry_value instanceof String) {
                        editor.putString(prefixed_key, (String) entry_value);
                    } else if (entry_value instanceof Integer) {
                        editor.putInt(prefixed_key, (Integer) entry_value);
                    } else if (entry_value instanceof Boolean) {
                        editor.putBoolean(prefixed_key, (Boolean) entry_value);
                    }
                }

                editor.apply();
            };


    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        setPreferencesFromResource(R.xml.per_app_preferences, rootKey);
    }

    @Override
    public void onResume() {
        super.onResume();
        getPreferenceManager().getSharedPreferences().registerOnSharedPreferenceChangeListener(listener);

    }

    @Override
    public void onPause() {
        super.onPause();
        getPreferenceManager().getSharedPreferences().unregisterOnSharedPreferenceChangeListener(listener);
    }
}

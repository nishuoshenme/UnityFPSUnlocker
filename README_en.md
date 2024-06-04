# UnityFPSUnlocker
## Installation requirements
- The device has already installed [Magisk](https://github.com/topjohnwu/Magisk/releases) / [KernelSU](https://github.com/tiann/KernelSU/releases) / [APatch](https://github.com/bmax121/APatch/releases)
- Enable `Zygisk` (For KernelSU / APatch user, Need install `Zygisk Next`)

## Additional requirements
- Enable `Shamiko` for the target game (Shamiko can avoid detected Zygisk injection or SafetyNet in some games)

## Use now
If your device does not have Zygisk enabled, please enable Zygisk first and then restart. After the above requirements are prepared, the 'UnityFPSUnlocker' module can be install in. Before rebooting, download `TargetList. json` and place it to `/data/local/tmp/TargetList. json`, and modify configuration by yourself. 
The module will automatically load by checking if the game data directory contains `/sdcard/Android/data/{package name}/files/il2cpp`.

```
{
  "global": {
    "delay": 10,
    "mod_opcode": true,
    "fps": 90,
    "scale": 1.0
  },
  "custom": {
    "com.random.package.name.a": {
      "fps": 60
    },
    "com.random.package.name.b": {
      "mod_opcode": false
    },
    "com.random.package.name.c": {
      "delay": 5
    }
  }
}
```

Among them, the configuration in the `global` node is:

- `fps` Required settings for `fps`，Set to `0` to disable module injection
- `delay` Wait for `delay` seconds to execute after the game is loaded
- `mod_opcode` Do you want to modify `opcode`, If you find that the game will lock fps again, you can change this to `true`, But due to modifying memory, it maybe detected by anti-cheat
- `scale` Set the multiplier of resolution, Generally, it keep `1.0`, and which must be a decimal number. `Current screen width * scale x Current screen height * scale`

Then, the configuration in the `custom` node will override the configuration in the `global` and take effect separately:

- `key` package name, like `com.random.package.name.a`
- `fps` Same as above
- `mod_opcode` Same as above
- `delay` Same as above
- `scale` Same as above

`TargetList.json` effective immediately after modification (`Module version>=1.8`)。  
And you can input `logcat - s UnityFPSUnlocker` in the shell to view the input logs.

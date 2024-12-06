# coralmicro_in_tree_pcb_bringup

Bring up code for edge human detection PCB. Includes code for the following:
- Read the TOF sensor
- Light up RGB LEDs with different colors
- Ouput serial messages

## Install

This repo includes a submodule for the TOF implementation code, so you need to clone it with the `--recursive` flag

```bash
git clone git@github.com:jc-cr/coralmicro_VL53L8CX_ULD_driver.git  --recursive
```

## Build the application

Update the project to the `app` directory CMakeLists.txt file:
```cmake
add_subdirectory(coralmicro_in_tree_pcb_bringup)
```

Now you should be able to build the application by running the following command:
```bash
bash build.sh
```

## Upload the application

To upload the application to the Coral Dev Board, you can run the following command:

```bash
python3 scripts/flashtool.py --app coralmicro_in_tree_pcb_bringup
```

## Run the application

I recommend using a USB to Serial adapter to connect to the Coral Dev Board. 
Using one your can view the serial output with the following command:

```bash
picocom -b 115200 /dev/ttyUSB0
```

Exit with:
```
Ctrl-a Ctrl-x
```
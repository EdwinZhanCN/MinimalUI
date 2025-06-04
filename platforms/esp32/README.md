# ESP32 SPI Documentation References

In this project, we use ESP-IDF `v5.4.1` as the Hardware Abstract Layer for ESP32 module.

## Linux and MacOS

### Ubuntu and Debian

```shell
sudo apt-get install git wget flex bison gperf python3 python3-pip python3-venv cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0
```

### MacOS

```shell
brew install cmake ninja dfu-util
```

You may make directory under your home folder

```shell
mkdir -p ~/esp
```

and clone the ESP-IDF repo as `esp-idf` folder

``` shell
cd ~/esp
# clone repo
git clone -b v5.4.1 --recursive https://github.com/espressif/esp-idf.git esp-idf
```

Add following command in the end of `~/.bashrc` file, you may use `vi ~/.bashrc` to edit it.

```shell
...
# ESP-IDF
export IDF_PATH="$HOME/esp/esp-idf"
alias get_idf='. $IDF_PATH/export.sh'
```

Get start with [中文文档](https://docs.espressif.com/projects/esp-idf/zh_CN/v5.4.1/esp32/get-started/index.html)
or [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/v5.4.1/esp32/get-started/index.html)

## SPI

Refer to [SPI Master Driver](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/spi_master.html)

# Watch firmware
for ttgo tdisplay

pinout of device:
|GPIO      |pin     |
|----------|--------|
|TFT Driver| ST7789 |
|TFT_MISO  |	N/A |
|TFT_MOSI  |	19  |
|TFT_SCLK  |	18  |
|TFT_CS    |    5   |
|TFT_DC    |	16  |
|TFT_RST   |	N/A |
|TFT_BL    |	4   |
|I2C_SDA   |	21  |
|I2C_SCL   |	22  |
|ADC_IN    |    34  |
|BUTTON1   |    35  |
|BUTTON2   |    0   |
|ADC Power |	34  |

shortcuts:

TTGO TDISPLAY reference:
https://github.com/Xinyuan-LilyGO/TTGO-T-Display

mkspiffs upload with:
```bash
pio run -t uploadfs
```

Refer to https://openweathermap.org/current
for weather data
import board
import busio
import adafruit_bmp280

scl_pin = board.SCL  # Pin 5 (GPIO3)
sda_pin = board.SDA  # Pin 3 (GPIO2)

# Initialisiere den I2C-Bus mit manuellen Pinzuweisungen
i2c = busio.I2C(scl_pin, sda_pin)

bmp280 = adafruit_bmp280.Adafruit_BMP280_I2C(i2c)

pressure = bmp280.pressure
temperature = bmp280.temperature

print(f"Druck: {pressure} hPa")
print(f"Temperatur: {temperature} C")
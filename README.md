ESP_NOW_Weather<br>
-----------------------------------
Copyright (c) 2023 BitBank Software, Inc.<br>
Written by Larry Bank<br>
bitbank@pobox.com<br>
<br>
The goal of this project is to experiment with ESP32 networking on a battery operated device to use the least power possible by leveraging 2 devices - one connected to mains power and the other powered by a battery. The mains power (server) will access
the internet, gather useful data and parse it into a compact representation to send to the client using ESP-NOW. In this case, weather data from wttr.in is read as JSON (> 60K) and parsed into a 36 byte structure sent in a single packet.
<br>
This photo shows the M5Stack AtomS3 acting as the server and the LaskaKit ESPInk acting as the client:
<br>
<br>
![e_paper_weather](/example.jpg?raw=true "e-paper weather")
<br>

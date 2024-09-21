# Sat
GNSS satellite positioning program

This program calculates the coordinates and velocity components of navigation satellites 
using data from ephemeris files in RINEX and .sp3 formats at an arbitrary point in time. 
The program allows you to evaluate the visibility of a certain group of satellites at the 
time of observation, and can be used in studies regarding the influence of satellite 
constellations on the accuracy of GNSS positioning, as well as for modeling the path 
of satellite radio signals through the Earth's atmosphere.

Main features of the program:
- support for airborne ephemeris files in RINEX format versions 2.xx and 3.xx, precise ephemeris files .sp3;
- control of source data;
- finding the coordinates and velocity components of satellites in a certain time interval with a given step;
- the ability to choose a method for calculating the speed of satellites - using ephemeris or by numerical differentiation of coordinates;
- in case of entering the coordinates of the receiver, the azimuth, elevation angle and geometric distance to the satellite at the time of sending the signal are calculated;
- output data to the screen and to a text file.

© Zatolokin D.A. 2015-2024
clausxxx@rambler.ru

#!/usr/bin/env python

# Munin plugin to parse temperatures from a Arduino over Serial
# Example response from the serial: 
# temp1: 25.5

import sys, serial, re

def gettemperature():
        ser = serial.Serial('/dev/tty.usbserial-A800eKiZ', 9600, timeout=1)
        temps = [None] * 3
        while (temps[0] is  None) or (temps[1] is  None) or (temps[2] is None):
                line = ser.readline().strip()
                if line is not None:
                  sensor_tmp = re.search("temp(\d+)",line)
                  if sensor_tmp is not None:
                    sensor = sensor_tmp.group()[4]
                    temp_tmp = re.search("(\d+)(\d+).(\d+)",line)
                    temp = temp_tmp.group()
                    temps[int(sensor)-1] = temp
        ser.close()
        return temps

if len(sys.argv) == 2 and sys.argv[1] == "autoconf":

  print "yes"

elif len(sys.argv) == 2 and sys.argv[1] == "config":

  print 'graph_title Temperature Autotalli'
  print 'graph_vlabel temperature in C'
  print 'graph_category System'
  print 'temperature1.label temperature1'
  print 'temperature2.label temperature2'
  print 'temperature3.label temperature3'
  print 'graph_info Tempereature'
  print 'graph_scale no'
  print 'graph_args --base 1000 -l 10 -u 50'

else:
  temps = gettemperature()
  for i, temp in enumerate(temps):
    print 'temperature'+str(i+1)+'.value %s' % temp

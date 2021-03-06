#!/usr/bin/env python

# Munin plugin to parse temperatures from a webpage that is generated by Arduino
# Example response from the HTML: 
# <meta http-equiv='refresh' content='10'/><title>Temp server</title><h1>Temp1: 20.8</h1><h1>Temp2: 20.0</h1><h1>Temp3: 21.0</h1>

# Big thanks https://github.com/aautio for helping with Python

# TODO url to munin parameter

import sys,re, urllib2

url = "http://192.168.2.40"

def gettemperature(response):
    return re.findall("Temp(\d):\s(-?\s*\d+.\d)", response)

if len(sys.argv) == 2 and sys.argv[1] == "autoconf":

  print "yes"

elif len(sys.argv) == 2 and sys.argv[1] == "config":

  print 'graph_title Temperature Olohuone'
  print 'graph_vlabel temperature in C'
  print 'graph_category System'
  print 'temperature1.label temperature1'
  print 'temperature2.label temperature2'
  print 'temperature3.label temperature3'
  print 'graph_info Tempereature'
  print 'graph_scale yes'
  # lower limit 10, upper limit 50
  print 'graph_args --base 1000 -l -25 -u 30'

else:
   response = urllib2.urlopen(url).read()
   temps = gettemperature(response)
   for id, temp in temps:
    print 'temperature'+id+'.value %s' % temp
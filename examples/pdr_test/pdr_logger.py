#! /usr/bin/python
from __future__ import division, absolute_import, print_function, unicode_literals

import sys as system
import os as os
import serial as serial
import shutil as shutil
import time as time
import struct as struct
import ctypes
import datetime
from collections import namedtuple

TEST_MESSAGE_COUNT = 100
FILE_NAME_PREFIX = "pdr_test_"
FILE_EXTENSION = ".csv"

serial_port = None
SerialData = namedtuple('SerialData', 'counter rssi')

def read_value_from_serial():
	data = serial_port.read(size=2)
	counter = struct.unpack("H", data)[0] 
	data = serial_port.read(size=1)
	rssi = struct.unpack("b", data)[0] 
	serialData = SerialData(counter, rssi)
	return serialData

def empty_serial_buffer():
	while serial_port.inWaiting() > 0:
		serial_port.read(1)

def main():
	if (len(system.argv) != 3):
		print("Usage: <serialport (eg COM7)> <baudrate (eg 9600)>")
		system.exit(2)

	global serial_port 
	serial_port = serial.Serial(system.argv[1], system.argv[2])
	get_input = input if system.version_info[0] >= 3 else raw_input # TODO compatibility beween python 2 and 3, can be removed if we switch to python 3 (waiting on matplotlib)
	test_name = get_input("Enter test name: ")
	f = open(FILE_NAME_PREFIX + test_name + FILE_EXTENSION, 'w')
	f.write("# distance (m), PDR (%), # packets, timestamp started" + "\n")
	f.write("distance, PDR, packet_count, started_timestamp\n")
	get_input("Press any key to start testing...")
	stop_testing = False
	while not stop_testing:
		empty_serial_buffer()
		initial_counter = read_value_from_serial().counter
		counter = initial_counter
		succes_msgs_count = 0
		error_count = 0
		total_msgs_count = 0
		while(total_msgs_count < TEST_MESSAGE_COUNT):
			serialData = read_value_from_serial()
			succes_msgs_count += 1
			new_counter = serialData.counter
			if(new_counter > counter + 1):
				error_count += new_counter - (counter + 1)
				print("!!! packet missed")

			counter = new_counter
			total_msgs_count = counter - initial_counter
			print("received counter value %(counter)i @ %(rssi)s dBm ==> %(pct)0.2f%% missed of %(total)i messages" % \
				{
					'pct': (error_count*100)/total_msgs_count, 
					'total': total_msgs_count,
					'counter': counter,
					'rssi': serialData.rssi
				})

		dist = float(get_input("Distance between sender and receiver (in m): "))
		pdr = (succes_msgs_count*100)/total_msgs_count
		print("test %(test_name)s with distance %(dist)0.2fm => PDR=%(pdr)0.2f%%" % \
			{
				'pdr': pdr,
				'dist': dist,
				'test_name': test_name
			})

		f.write("%(dist)0.2f, %(pdr)0.2f, %(total)i, %(timestamp)s\n" %
			{
				'dist': dist,
				'pdr': pdr,
				'total': total_msgs_count,
				'timestamp': str(datetime.datetime.now())
			})
		
		f.flush()

		keypressed = get_input("Press any key to start next test, 'q' to stop testing ... ")
		if keypressed == "q":
			stop_testing = True

if __name__ == "__main__":
	main()
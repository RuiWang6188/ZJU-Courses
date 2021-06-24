1. There're 5 files in this folder. "header files" is the header file needed in this project.  "Project3.c" is the source code and  "Project3.exe" is the executable file of this project, "Max_juxtaposition" and "Max_N" are boundary input sample which you can test on you computer.

2. To build the project, you need to include the header files in the "header files" to the project, as well as the .c code "Project3".

3. Input Specification:
	Each input file contains one test case. Each case starts with two positive integers N, the number of records, and K，the number of queries. Then N lines follow, each gives a record in the format:

				plate_number hh:mm:ss status

where plate_number is a string of 7 English capital letters or 1-digit numbers; hh:mm:ss represents the time point in a day by hour:minute:second, with the earliest time being 00:00:00 and the latest 23:59:59; and status is either in or out.
	Note that all times will be within a single day. Each in record is paired with the chronologically next record for the same car provided it is an out record. Any in records that are not paired with an out record are ignored, as are out records not paired with an in record. It is guaranteed that at least one car is well paired in the input, and no car is both in and out at the same moment. Times are recorded using a 24-hour clock.
	Then K lines of queries follow, each gives a time point in the format hh:mm:ss. Note: the queries are given in ascending order of the times.
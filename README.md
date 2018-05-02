# volunteer
simple volunteer assignment tool

Steps to run:
copy list of hosts from miqaat sheet in hosts.txt
copy list of dates from miqaat sheet in dates.txt
copy list of volunteers in shabab.txt (note the names of people should be same format/spelling in hosts.txt and shabab.txt - else if it does not match soem volunteers might get assigned on day of their jaman)
modify the defines in config.h appropriately
compile and run the .cpp file and it should generate a csv containing three columns of JV

Note:
All the decision logic is in the canAssign() function

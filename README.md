# Report 
Check out my "report.pdf" file for program screenshots and explaination of each phase.

Also, a readme.txt file containing compilation details/instructions has been added.

# Summary 
Design and implement an Email system event modeller & intrusion detection system in accordance with the system descriptions below. 
My program has been implemented is C++. 

# Components 
• Initial input.

• Activity engine and the logs.

• Analysis engine.

• Alert engine.


# Initial Input

Events.txt and Stats.txt define the formats and the distributions of the events to be modelled. Days is an integer used in the next section.
Here goes an example Events.txt file. This file describes the events and some of their parameters.

5
Logins:D:0::3:

Time online:C:0:1440:2:

Emails sent:D:0::1:

Emails opened:D:0::1:

Emails deteled:D:0::2:

The first line contains the number of events being monitored. Each subsequent line is of the form
Event name:[CD]:minimum:maximum:weight:
C and D represent continuous and discrete events respectively. Discrete events takes integer values and occur one at a time, continuous events don’t need to take an integer value and an occurrence of that event may be of any value. The minimum and maximum specify the allowed range for that event type across a day. Continuous events need to be recorded to two decimal places. The weights are used in the alert engine and will always be positive integers.
The file Stats.txt contains the distributions to be modelled for the events. 

Here goes an example Stats.txt file: 

5

Logins:4:1.5:

Time online:150.5:25.00: 

Emails sent:10:3:

Emails opened:12:4.5: 

Emails deteled:7:2.25:

The first line again contains the number of events being monitored. Each subsequent line is of the form
Event name:mean:standard deviation:



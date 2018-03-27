# PUT-Projects
Projects created as additional work for classes on Poznan University of Technology.
Without refactorings and clean code standards. Little shame to show it but I do not have anything else at this moment and this is better than nothing.

## Projects
### Private Lessons Planner - Old Console Version (C++)
The program generates an timetable(MS Excel) for the tutor based on his availability and availability of clients. Uses the greedy algorithm.
Chooses the best plan out of twenty based on profit that it gives tutor.

**Pros:** 

*It generates really good timetable

**Cons:** 

*The code is little mess, need refactoring and must be better designed with design patterns.

**Improvements to be done in future:** more flexible architecture with design patterns, use SQLite to work with data, GUI based on Qt library

### TCP Multithreaded Server - Binary Protocol (C#)
The client-server architecture program allows multiple clients to connect server and send requests to calculate mathematical operations. Client and
server communicate using authorial binary protocol about the size of 14 bytes.

**Protocol characteristics:**
- Operation: substraction, division, addition, average (2 bits)
- First number (32 bits)
- Second number (32 bits)
- Third number (32 bits)
- Status: OK, Overrange, Underrange, Division by 0 (4 bits)
- Session ID (6 bits)
- INIT bit (1 bit)
- Unused bits (3 bits)

**Pros:**
* Unlimited number of clients(theoretically).
* Protected from wrong mathematical results.

**Cons:** 
*Client is not protected from exception errors(i.e. user type string of characters instead of number)

### UDP Multithreaded Server - Text Protocol (C#)
The client-server architecture program allows three clients(multiple clients can try, but only three can play) to play the guessing number with time limit. 

**Pros:** 
*Every 10 seconds sends information of how much time is left
*Retransmission of lost datagrams

**Cons:** 
*Buggy after first game


## IDE

*Microsoft Visual Studio 2015


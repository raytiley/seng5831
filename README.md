#Ray Tiley Lab 1

##Question 1

Using an iPhone to time the 60 duration of the experiment I ended up with the following output. 

```terminal
Toggles R:56 G:63 Y:63
````

I did however run this expierement before some changes to the menu code and obtained better results (the Red toggle was closer to 60). The previous results were obtained before menu code echoed back out each character received. In addition I changed the menu code to require a bang character to initiate a command. I think that the additional serial communication causes the initial zeroing of the counters to interfere with the busy wait loop.

This demonstrates exactly why we were asked to not enter in any menu commands during the expierement. Since the menu is interupt driven it steals processing cycles away from the busy wait loop used to time the red LED. This in turn increases the WCET for the red LED task and causes it not to execute as often as it should.


##Question 2

I performed the experiment twice both times using an iPhone as a stop watch. Both trials produced toggle counts that were within one of each other.

*Trial 1*
```terminal
Toggles R:59 G:60 Y:59
```

*Trial 2*
```terminal
Toggles R:59 G:60 Y:60
```

##Question 3

Placing a busy wait in the green LED's ISR affected the red and yellow software timers. The results are below. The red and yellow both toggled only about 83% of the expected toggles.

```terminal
Toggles R:97 G:122 Y:99
```
Busy waiting inside of the hardware timer steals CPU cycles from the software timers. This caues them not to increment there counters as often as they should and therfore slow down compared to the hardware timer.

When placing the 90ms busy wait into the yellow LEDs the menu functions became unusable. I'm not positive I was even able to set the period to 2Hz. The green led continued to blink regularly. The yellow LED would blink eventually, but would be much slower than the green. The green LED would toggle 5 times in the time it took the yellow LED to toggle once. The red led would not blink at all.

Placing the busy wait inside the yellow LED's ISR stole many more CPU cycles from the system than placing the same busy wait in the green LED's ISR. This is because the yellow LED's ISR is executed at a constanct frequency (10Hz) regardless of the user defined period of the other system tasks. This causes the system to operate irregularly. As I sit here and type this I very slowly see some of the menu commands finally being echoed back.

Manually counting the blinking lights during a 60 second interval gave 60 toggles for Red and 6 for yellow.

##Question 4

Placing a 110ms busy wait in the green ISR caused the red and yellow software timers to slow down even further. The green led toggled exactly as many times as its supposed to.

```terminal
Toggles R:91 G:121 Y:94
```

Placing a 110ms busy wait into the yellow ISR caused the yellow to only toggle 5 time in a 60 second period. The green led toggled 60 times exactly as it should. The red led did not function at all and the menu at this point was completely unusable.

##Question 5

Placing a 510ms busy wait in the green ISR casued the menu to be unusable. It also caused the red LED not to blink at all, and the yellow to come on and stay on, no toggling. I imagine this is mostly due to the timing of when I issued the command to swith to a 2Hz period. Depending on what state the yellow and red leds are in when the period changes to 2Hz they will likely stay in those states since the green ISR is essentially using all the cpu.

With a 510ms busy wait in the yellow ISR the yellow only toggled once during a 60 second interval. The green LED continued to blink normally. The red LED did not blink at all. 

##Question 6

With sei() at the top and a 510ms busy wait in the green ISR the yellow LED once again toggled normally at the same rate as the green LED. The red led however was stuck on after about two toggles once switching to a 2hz period. 

Running the same test a second time, this time initializing the periods to 2Hz during system start the red led did not come on at all. It also seems that the yellow LED is slightly slower then the green led.

Placing an sei() at the top and a 510ms busy wait in the yellow LED caused causes strange behavior. Inititally it would cause the system to crash and reinitialize every 10 seconds. After much trial and error I finally realized that this might be caused by allocating an integer inside the ISR to keep track of the busy wait cycles. 

Debugging showed me that code after the busy wait was never being executed. This is expected because the ISR is called 10 times a second, and the busy wait takes over half a second. Because the ISR is activating the interrupts, it is allowed to interrupt itself causing it to never finish. 

My hypothesis is the allocated integer in each ISR call eventually overflowed the stack. Switching to a global variable that is set to zero during the start of every busy wait made the crash happen much less often. It however did still happen occasionaly, which makes me not totally sure of my theory.

##Postmortum

I'm calling this section a postmortum because I did a lot of messing around during the last experiment and wanted to share the thoughts, but not necessarily redo all the experiments.

*My timers weren't always set to 500ms.* I initially had all the timers default to a period of 1000ms. After experiment two I couldn't use the menu code, and it wasn't until question 6 that I finally decided to inititalize all the periods to 500ms. This means that some of my counts might seem low for a 60 second test, but its only because I couldn't change the period due to broken menu controls, and didn't go back and rerun the experiments.

*Debugging ISRs is complicated* My biggest take away from this assignment is that doing anything non-trivial inside an ISR can lead to unexpected behavior. I've already covered in depth my findings about allocating an integer inside an ISR that can never finish, but during this debugging phase I also tried using the LCD to debug. It turns out that using the LCD could affect the behavior of the original problem I was trying to debug.
#Ray Tiley Lab 1

##Question 1

Using an iPhone to time the 60 duration of the experiment I ended up with the following output. 

```terminal
Toggles R:56 G:63 Y:63
````

I did however run this expierement before some changes to the menu code and obtained better results (the Red toggle was closer to 60). The previous results were obtained before menu code echoed back out each character received. In combination with the change I made to the menu code to require a bang character to initiate a command I think that the initial zeroing of the counters interferes with the busy wait loop. 

This demonstrates exactly why you asked us not enter in any menu commands during the expierement. Since the menu is interupt driven it steals processing cycles away from the busy wait loop used to time the red LED. This in turn delays increases the WCET for the red LED task and causes it not to execute as often as it should.


##Question 2

Again timed with an iPhone all the outputs were within one toggle of each other.

```terminal
Toggles R:59 G:60 Y:59
```
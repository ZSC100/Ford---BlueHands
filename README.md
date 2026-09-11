# Ford---BlueHands
Vehicle hack to inject steering wheel button presses that eliminate the ADAS steering wheel torque nag on Gen14 F-150 trucks.

This is a prototype for testing; use at your own risk.

Hardware: https://copperhilltech.com/teensy-4-0-can-fd-and-lin-bus-breakout-board/

The Source code and Arduino IDE is not needed to flash your board, only if you want to modify and compile you own.

If you simply want use this hack, just download the Teensy Loader: https://www.pjrc.com/teensy/loader.html
and load the hex file to flash your board.

Pictures of the CAN FD tap method are included as well as Ford's wiring diagram for the Ford Lightning.  For Makers/EE's it's simply bread board hookup wire, I have rolls of these 10 colors on my electronics workbench spool holder.

Digikey Part #'s:
White: C2004W-100-ND  
Blue: C2004L-100-ND  
Black: C2004B-100-ND  
Red: C2004R-100-ND  

This wire and a few small zip ties is all you need to hook up(tap) the C242B connector to the Copperhill board.

C242B Wire Tap Key:

Pin 7: CAN-FD Low, Blue  
Pin 8: CAN-FD High, White

Pin 9: CCM ECU -12V, Black  
Pin 18: CCM ECU +12V, Red

<body>

<div align="left">

<table border="0" width="100%" cellspacing="0" cellpadding="0" id="table1">
	<tr>
		<td><font face="Courier">
		------------------------------------------------------------------------------</font><p>
		<font face="Courier">This is a nonblock step motor (28byj-48) library 
		based on interrupts<br>
		By Antonio Testa, March/2024<br>
		<br>
		Special thanks to @gfvalvo from https://forum.arduino.cc who fixed the 
		error: invalid-use-of-non-static-member-function and gave all tricks to 
		use hardware timers inside the library<br>
		<br>
		https://forum.arduino.cc/t/error-invalid-use-of-non-static-member-function/1240240<br>
		<br>
		------------------------------------------------------------------------------<br>
		In the main program is mandatory to define the following:<br>
		<br>
		#include &lt;Stepm.h&gt;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; //include the NewStep.h 
		elibrary <br>
		<br>
		Stepm name(5,17,16,4);&nbsp; //define instance for ULN2003 driver motor 
		pins in1,in2,in3,in4<br>
		<br>
		name.begin(mode);&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp; //At setup(), 
		issue begin() command to init the library timer and set stepmotor mode 
		operation<br>
		------------------------------------------------------------------------------<br>
		<br>
		CONSTRUCTOR:<br>
		<br>
		<b>Stepm name(pin4, pin3, pin2, pin1); </b><br>
		name is any name for instance stepmotor<br>
		uint8_t pin4,3,2,1 reffers to ESP-32 pins connected to in4,3,2,1 of 
		ULN2003 motor driver<br>
		<br>
		<br>
		METHODS:<br>
		<br>
		<b>name.begin(uint8_t mode);</b><br>
		at setup() section, to init the library timer and step motor mode 
		operation<br>
		uint8_t mode should be:<br>
		0=full step, low torque, low consumption<br>
		1=full step, high torque, high consumption<br>
		2=half step, middle torque, middle consumption <br>
		<br>
		<b>name.run(steps,vel,cw);</b><br>
		uint32_t steps is the number of steps to travel (512 steps per turn)<br>
		uint8_t vel is the speed (max 3 for right direction, 5 for left 
		direction) increase value means decrease speed<br>
		boolean cw define direction of turn (true for right, clockwise; false 
		for left, counterclockwise)<br>
		IMPORTANT: the name.run can only be called if the name.where()==0 <br>
		<br>
		<b>name.where();</b><br>
		retun in an uint32_t value the number of steps remainding to motor 
		arrives to destination<br>
		if name.where()==0 means the motor is already in the last destination 
		and is stopped at this moment<br>
		<br>
		<b>name.setms(uint32_t ms);</b><br>
		set the ms counter down (nonblock to be used in substitution of delay 
		function).<br>
		As soon as initiated this ms counter is decremented each millisecond<br>
		<br>
		<b>name.getms();</b><br>
		return the ms counter at moment (initial ms value setted by previously 
		name.setms)<br>
		------------------------------------------------------------------------------<br>
		<br>
		//<br>
		// library Stepm.h<br>
		// Simple example of use<br>
		// move 4096 steps forward (clockwise, right) following by 4096 steps 
		backward (counterclockwise, left)<br>
		// <br>
		// function used:<br>
		// begin() - init the timer library<br>
		// where() - returns the number of steps remainding to stepmotor gets to 
		it's destination<br>
		// run(steps,speed,direction) - cause step motor to move according to 
		parameters informed<br>
		// <br>
		<br>
		#include &lt;Stepm.h&gt;<br>
		<br>
		Stepm sm(5,17,16,4);&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; //define constructor for 28byj-48 and ULN2003 
		driver motor pins in1,in2,in3,in4<br>
		<br>
		void setup()<br>
		{<br>
&nbsp; sm.begin(2); //define the step motor mode operation (always 4096 steps 
		per turn) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp;&nbsp; 
		//0=full step, low torque, low consumption<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp; 
		//1=full step, high torque, high consumption<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp; 
		//2=half step, middle torque, middle consumption<br>
		<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp; 
		//the max speed depending on mode selected:<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp; 
		//mode 0 - max speed 3 for clockwise and 5 for counterclockwise<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp; 
		//mode 1 - max speed 3 for clockwise and 5 for counterclockwise<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp; 
		//mode 2 - max speed 2 for clockwise and 2 for counterclockwise<br>
		}<br>
		<br>
		void loop()<br>
		{<br>
&nbsp; while (sm.where()&gt;0){}&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
		//wait until step motor arrives to its last destination<br>
&nbsp; sm.setms(500);while (sm.getms()&gt;0){}&nbsp;&nbsp; //wait 500ms<br>
&nbsp; sm.run(4096, 2, true);&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; //start 4096 steps forward (clockwise, right) <br>
&nbsp; while (sm.where()&gt;0){}&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; //wait until step motor arrives to its last destination<br>
&nbsp; sm.setms(500);while (sm.getms()&gt;0){}&nbsp; //wait 500ms<br>
&nbsp; sm.run(4096, 2, false);&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; //start 4096 steps backward (counterclockwise, left) <br>
		}<br>
		<br>
		------------------------------------------------------------------------------<br>
&nbsp;</font></p>
		<p>&nbsp;</td>
	</tr>
</table>

</div>

</body>

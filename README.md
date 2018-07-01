# Formula

A formula module for VCV Rack, by Frank Buss, based on BokontepByteBeatMachine.

This plugin provides 3 inputs: x, y and z. In the text field you can write a formula for the output.
E.g. x+y would be a simple adder. Also supported are a range of standard math functions, known from the C programming language:

acos, asin, atan, atan2, cos, cosh, exp, abs, mod, log, log10, pow, sin, sinh, tan, tanh, sqrt, ceil, floor, max, and min

And there is the constant pi defined. So if you feed a sawtooth with an amplitude from -5 to 5 to x, you will get a sine wave at the output
with the formula sin(2*pi*x/5)*5, as you can see here:

![alt text](formula.png "Formula on VCVRack")

Some functions take 2 arguments. For example you could use the max function, to get either the input x or the input y, depending on which
voltage is higher: max(x, y).

The red LED is blinking if there is a parsing error. If it is on, the formula is running.

There are also relational and logical operators defined. So you could write a function, which outputs the signal y, if x is
less than 2.5 V, or outputs z, if x is 2.5 V or greater. The "true" value is -1, "false" is 0. This means the formula could look like this:
(x<2.5)*y+(x>=2.5)*z

You can read about the full documentation of the formula syntax here: http://www.frank-buss.de/formula/index.html

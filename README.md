# ⚓ What's "Ship"?
Ship is an interpreted programming language made from scratch with a pirate thematic syntax. It's dynamic typed and can handle basic expressions and statements. It's also easily moddable and totally open-source with the apache license.

## Installation
To install and use the ship interpreter, you must have a c++ compiler already installed in your system and then build the file "main.cpp". Example using g++:<br>
`$ g++ main.cpp -o ship`<br><br>
If you're on linux, you can now move the built main.cpp to the folder `/usr/bin/` and use it anywhere.

## Usage
Currently, you can use it in command line mode (run the executable in the terminal) or reading a file passing the file name as an argument. Example: `$ ./ship script.ship`

## Examples
Here is an example program built with Ship.

- circumferenceCalc.ship
```
yell("## Circumference Calculator Program ##");
grrr PI = 3.14159;
yarr radius = hail("What's the radius is value? ");
radius = toNum(radius);
grrr circumference = (2 * PI) * radius;

yell("The circumference is", circumference);
```

yell -> work as print()<br>
grrr -> defines a constant<br>
yarr -> defines a mutable<br>
toNum() -> turn strings into numbers (toStr() does the opposite)<br>

Those are just few keywords, if you want to see more, check the `runtime/nativeMaps.h`.

## Thank you!
This is a project made for fun only and shouldn't get much attention, thanks for taking a look.
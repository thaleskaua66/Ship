# What's "Ship"?
Ship is an interpreter made in C++ to run ".ship" files. Ship has a pirate thematic syntax.

## Under Development
Ship is still under development. For now, you can create constant or mutable variables (using yarr or grrr), use simple built-in functions (see runtime/nativeMaps.h) such as yell(<something>) which works as a std::cout << <something> << '\n'.

## Examples
Here is an example program built with Ship.

- circumferenceCalc.ship
```
grrr PI = 3.14159;
grrr radius = 5;

grrr circumference = (2 * PI) * radius;

yell(circumference)
```

Not that you can't use a semicolon after `yell(circumference)`. This will be fixed as fast as possible and all lines will need a semicolon on it's end.

## Thank you!
This is a project made for fun only and shouldn't get much attention, thanks for taking a look.
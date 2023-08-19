# PSL
A collection of things that polar finds useful in c++. Often these are remakes of existing things because I'm a c++ dev, so I legally can't use libraries.

### How to make argument serializer
It should take a defined struct, and then a function registering all the fields of the struct as either command line, json, or both. It will also have priority of which 
one to obey.

Due to the abuse of pointers it will most likely break the config parser if built for a platform that is not 64 bits.
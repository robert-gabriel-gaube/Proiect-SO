# Guide to installation and building

## Installation guide
Steps to get the repository up and running. We need to setup submodules for the Unity testing framework.

1. ```git clone <url>```
2. ```cd Proiect-SO```
3. ```git submodule init```
4. ```git submodule update```

## How to run release

1. ```mkdir build && cd build```
2. ```cmake ..``` 
3. ```make```
4. ```./program <input_file>```
 
## For debbuging or running tests

1. ```mkdir build && cd build```
2. ```cmake -DDEBUG=[ON/OFF] -DBUILD_TESTS=[ON/OFF] ..``` 
3. ```make```
4. ```./program <input_file>```
5. ```ctest``` (If BUILD_TESTS is enabled)

You can either enable both flags or only one: 
- **DEBUG** - This option prints debug information when the application is executed.
- **BUILD_TESTS** - This option builds the unit tests and enables ctest.

TIP: You can use ```ctest --verbose``` for mode detailed information when tests are executed.



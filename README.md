# Guide to installation and building

## Installation guide
Steps to get the repository up and running. We need to setup submodules for the Unity testing framework.

1. ```git clone <url>```
2. ```cd Proiect-SO```
3. ```git submodule init```
4. ```git submodule update```

## How to run

Create a build folder and use cmake with different flags to build the application. \
If **DEBUG=ON**, some debug messages will appear in the console when the application is executed. \
If **BUILD_TESTS=ON**, you will be able to run unit tests.

1. ```mkdir build && cd build```
2. ```cmake -DDEBUG=[ON/OFF] -DBUILD_TESTS=[ON/OFF] ..``` 
3. ```make```
4. For running the application: ```./program <input_file>```
5. For running unit tests: ```ctest``` or ```ctest --verbose``` for more details.
 
Example for 'Release' mode would be: ```cmake -DDEBUG=OFF -DBUILD_TESTS=OFF ..``` or you can just write ```cmake ..``` because they are defaulted as OFF. 

Example for 'Debug' mode would be: ```cmake -DDEBUG=ON -DBUILD_TESTS=ON ..```.


# Fake clicker
A programm to create a fake clicker with an arduino and nrf24l01 module.
- [Fake clicker](#fake-clicker)
	- [Wiring](#wiring)
	- [Arduino serial commands](#arduino-serial-commands)
	- [Qt program usage](#qt-program-usage)

## Wiring
![arduino wiring image](https://raw.githubusercontent.com/Robotechnic/fakeClicker/master/images/wiring.svg)

| Arduino Pin | nrf24l01 Pin |
| :---------: | :----------: |
|    3.3v     |     vcc      |
|     gnd     |     gnd      |
|      8      |     CSN      |
|      7      |      CE      |
|     13      |     SCK      |
|     11      |     MOSI     |
|     12      |     MISO     |

## Arduino serial commands
1. Commands
   
|                Command                 |                               Usage                               |
| :------------------------------------: | :---------------------------------------------------------------: |
|            `send <number>`             |  send a number between 0 and 9 included with the current adress   |
|           `channel <number>`           | Set the channel to desired number. Min channel is 1 and max is 81 |
| `adress <number1> <number2> <number3>` |              Set the 3 bits of the clicker's adress               |

2. Errors messages

|      Error      |                Meaning                 |
| :-------------: | :------------------------------------: |
| argMustBeNumber | one of the provided arg isn't a number |
|   argRequired   |          one arg is required           |
|  nargsRequired  |          n args are required           |


## Qt program usage
*Comming soon*
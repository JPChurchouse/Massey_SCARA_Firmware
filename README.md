# SCARA Firmware

![SCARA](scara.jpg | width=400)

### Commands

| Name | Command | Parameters | Returns | Description | Example |
|-|-|-|-|-|-|
|Stop Robot | STOP | - | STOPPED | Stop the robot immediately. This command is processed differently to all other commands in that it is processed on receive rather than added to a buffer. | STOP -> STOP |
| Ping | PING | - | PONG | Ping the device to test communication. | PING -> PONG|
| Home | HOME | - | HOME | Home the robot to 0,0,0 | HOME -> HOME|
| Move to | MOVE | ,x,y,w (int, int, int) | MOVE: x y w | Move to coordinates x,y and rotate the EE w. |  MOVE,37,89,20 -> MOVE 37 89 20|
| Air control | AIR | ,c[,w] (char, [uint]) | AIR: c[ d] | Activate air function c with optional delay d. | AIR,U,1000 -> AIR U 1000 |
| Wait for | WAIT | ,w (uint) | WAIT: d | Wait a time in milliseconds, w, before proceeding. | WAIT,500 -> WAIT 500 |
| Ding | DING | ,msg (string) | DONG: msg | Send a message, msg, when the robot reaches that command. | DING,HelloWorld -> DING HelloWorld |
| Clear buffer | CLEAR | - | CLEAR | Clear the buffer. | CLEAR -> CLEAR |
| Echo commands | ECHO | ,b (uint as a bool)| ECHO b | Turn on/off (b) confirmation of command receive messages. | ECHO,1 -> ECHO 1 |
| Set speeds | SPEEDSET | ,v,a (uint, uint) | SPEEDSET: v a | Set the velocity, v, and acceleration, a, values of the robot. | SPEEDSET,100,80 -> SPEEDSET 100 80 |
| Negative acknowledge | - | - | NACK: cmd | Command was received, cmd, but is invalid. | I_Love_Massey -> NACK I_Love_Massey |

![](logo.png)
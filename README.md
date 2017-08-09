# Lab 1: Hello World with Johnny Five

## Objective

The objectives of this lab are:

1. Create a Hello World application using Johnny Five.
2. Blink an LED on Arduino Uno board while being controlled by a client.

![Lab 1 Objectives](https://maxmiaggi.github.io/johnnyfive-iot-class-masters2017/Images/lab1_objectives.png)

## Step 1 - System Setup

This step is required to install all the necessary files and set up the system.

**Note:** This step has already been performed on your computer for MASTERs. This step is included to give you an overall perspective. You can follow these steps to set up your personal computer later on.

1. Install NodeJS (LTS version) from https://nodejs.org/ 
2. Install Johnny Five via Node Package Manager (NPM) through Command Prompt.
  ```
  npm install johnny-five
  ```    
3. Install PubNub via NPM through Command Prompt (**Note**: PubNub is not used in this lab. It will be used in next lab).
	```bash
	npm install pubnub
	``` 

## Step 2 - Burn Firmata onto Arduino board

1. Log into [Arduino Create](https://create.arduino.cc/) and click on **Arduino Web Editor**.

	![Arduino Create](https://maxmiaggi.github.io/johnnyfive-iot-class-masters2017/Images/arduino_create.png)

	![Arduino Web Editor](https://maxmiaggi.github.io/johnnyfive-iot-class-masters2017/Images/arduino_web_editor.png)
    
2. Install the [Arduino Web Editor Plugin](http://create.arduino.cc/getting-started/plugin). **This is already done for MASTERs computers. Go to next step.**

	![Arduino Plugin Install](https://maxmiaggi.github.io/johnnyfive-iot-class-masters2017/Images/arduino_plugin_install.png)

3. Connect your Arduino Uno board to the computer. Arduino Create should now automatically detect the COM port the board is connected to.
4. Upload Firmata onto Arduino by following 5 simple steps as shown below:

	![Upload Firmata](https://maxmiaggi.github.io/johnnyfive-iot-class-masters2017/Images/upload_firmata.png)
   
That's it for Arduino side. You can now close Arduino Create - we won't need it any more for this class.
    
## Step 3 - Client side code using Johnny Five
1. Create a file blink.js and type the following code (or navigate to the Labs folder and open the code):

    ``` javascript
    var five = require('johnny-five');
    var board = new five.Board();
    // Specify COM port if needed 
    // var board = new five.Board({ port: "COM14" });

    board.on('ready', function() {
      var led = new five.Led(13); // pin 13
      led.blink(500); 			  // 500ms interval
    });
    ```

1. Run the code by executing the following command:

    ```
    node blink.js
    ```





















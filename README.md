# Lab 1: Hello World with Johnny Five

## Objective

The objectives of this lab are:

* Create a Hello World application using Johnny Five.
* Blink an LED on Arduino Uno board while being controlled by a client.

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
	```
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

2. Run the code by executing the following command:

    ```
    node blink.js
    ```
You should be able to see the onboard LED blink every 500 ms. Notice that the Rx and Tx LEDs on the Arduino board light up every time the LED blinks. This reinforces the fact that the code is actually running on the computer and the computer is communicating with the Arduino via UART sending/receiving commands to turn the LED on/off.


# Lab 2: Hello World of IoT

## Objectives

The objectives of this lab are:

* Create a Hello World application using IoT concepts
* Blink an LED on Arduino Uno board being controlled by a remote Client over the web

![Lab 2 Objectives](https://maxmiaggi.github.io/johnnyfive-iot-class-masters2017/Images/lab2_objectives.png)

For this lab, we are using [PubNub](https://www.pubnub.com/) as the MQTT broker.

## Browser-side Code

Since we are controlling a remote hardware over the web, we need to have a GUI on the web to publish commands. The browser-side GUI requires HTML, CSS and JavaScript.

We are going to create a very simple web page which will have a simple button. When we click on the button, it publishes blinkState to PubNub (MQTT broker). You can use [Codepen](https://codepen.io/pen/) to type in the code.

**File Name: blink-remote-client-html.html**

### HTML Code

The following HTML code creates a very simple button.

```html
<!-- Heading of the web page -->
<h1>Prototyping IoT Demo UI</h1>

<!-- This creates the button with text "Blink LED" on it -->
<button class="on">Blink LED</button>

<!-- This calls the pubnub script running on a remote server -->
<script src="https://cdn.pubnub.com/sdk/javascript/pubnub.4.13.0.min.js"></script>
```

### CSS Code

The following CSS code applies very simple styling to the button we created above.

**File Name: blink-remote-client-css.css**

```css
/* This styling applies to all buttons on the page */
button {
  font-size: 2em;
  padding: 10px 20px;
}
```

### JavaScript Code

The following JS code makes the button we created earlier functional.

**File Name: blink-remote-client-js.js**

```javascript
// Initialize PubNub
var pubnub = new PubNub({
  // Use your own keys
  subscribeKey: "sub-c-801ccfbc-...", // always required
  publishKey: "pub-c-cfd2c879-..." // only required if publishing
});

// Declare global variables
var channel = "led";
var button = document.querySelector("button.on");
var blinkState = true;

// This function responds to clicking the button
button.addEventListener("click", function(e) {

  // Publish "blinkState" to the specified channel
  pubnub.publish(
    {
      channel: channel,
      message: { blink: blinkState }
    },
    // Callback function after publishing
    function(m) {
      // Log in the console
      console.log(m);
      
      // Toggle blinkState
      blinkState = !blinkState;
      
      // Change text on the button
      button.textContent = blinkState ? "Blink LED" : "Stop LED";
    }
  );
});
```

### All-in-one Code

If you want to host the web page on your own server, you will need to create a single HTML file and include all your HTML/CSS/JS in that one file, or create separate .html, .css, and .js files and call them from the HTML file. Here's how a single HTML file would look like:

**File Name: blink-remote-client.html**

```html
<!DOCTYPE html>
<html>
<head>
  <title>Remote LED Blink</title>
  <script src="https://cdn.pubnub.com/sdk/javascript/pubnub.4.13.0.min.js"></script>
  <style type='text/css'>
    button {
      font-size: 2em;
      padding: 10px 20px;
    }
  </style>
</head>

<body>

  <h1>Prototyping IoT Demo UI</h1>
  <button class="on">Blink LED</button>


  <script type='text/javascript'>
    var pubnub = new PubNub({
      subscribeKey: "sub-c-801ccfbc-...", // always required
      publishKey: "pub-c-cfd2c879-..." // only required if publishing
    });

    var channel = "led";
    var button = document.querySelector("button.on");
    var blinkState = true;

    button.addEventListener("click", function(e) {
      pubnub.publish(
        {
          channel: channel,
          message: { blink: blinkState }
        },
        function(m) {
          console.log(m);
          blinkState = !blinkState;
          button.textContent = blinkState ? "Blink LED" : "Stop LED";
        }
      );
    });

  </script>
</body>
</html>
```


## Server-side Node.js

On your server (the host computer), you'll need to write a JavaScript code using Johnny Five library to perform the following functions:

- Subscribe and listen to messages from MQTT broker (PubNub)
- Talk to the microcontroller/Arduino4

**File Name: blink-remote.js**

```javascript
// Include Johnny Five library
var five = require('johnny-five');

// Initialize board (specify COM port if J5 cannot find it)
// var board = new five.Board({ port: "COM14" });
var board = new five.Board();

// Initialize PubNub
var PubNub = require("pubnub");
var pubnub = new PubNub({
    ssl          : true,  // <- enable TLS Tunneling over TCP
    publishKey   : "pub-c-cfd2c879-...",  // use your own keys
    subscribeKey : "sub-c-801ccfbc-..."
});

// Specify which topic/channel to subscribe to
var channel = 'led';

// Everything inside here runs on the J5 initialized board
board.on('ready', function() {
  // LED on pin 13
  var led = new five.Led(13);

  // Listener listens to subscribed messages and executes
  // whenever there is data available on the channel
  pubnub.addListener({
    message: function(m) {
      // Check for received message. If true, blink. Otherwise LED off.
      if(m.message.blink == true) {
        led.blink(500);
      } else {
        led.stop();
        led.off();
      }
    }
  });

  // Subscribe to a channel
  pubnub.subscribe({
    channels: [channel],
  });


});
```




































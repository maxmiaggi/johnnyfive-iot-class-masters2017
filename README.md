# Getting Started with Johnny Five - The JavaScript Robotics and IoT Platform - Official Lab Manual

This web page acts as a guide for the labs for this Evening Class as Microchip MASTERs 2017.

*Contact Information:
Max Prasad
Max.Prasad@microchip.com*

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

## Screenshot


![Lab 2 Screenshot 1](https://maxmiaggi.github.io/johnnyfive-iot-class-masters2017/Images/lab2_screenshot_1.png)
![Lab 2 Screenshot 2](https://maxmiaggi.github.io/johnnyfive-iot-class-masters2017/Images/lab2_screenshot_2.png)

## Working

When you click on the "Blink LED" button, the browser will publish the status to the PubNub's data stream network. The host computer connected to the Arduino will read the status and respond accordingly.

# Lab 3: IoT Sensor Data Visualization

## Objectives

The objectives of this lab are:

- Read sensor data from Arduino and publish to web
- Subscribe to the sensor data via a web page and create a visual using charts

![Lab 3 Objectives](https://maxmiaggi.github.io/johnnyfive-iot-class-masters2017/Images/lab3_objectives.png)

In this lab, we are using Johnny Five to read data from 3 different sensors connected to the Arduino board. The sensors used are:
- MCP9808 I2C Temperature Sensor
- Photoresistor
- Potentiometer

This data is then published to the MQTT broker. There's a web page subscribed to this channel that is listening to the values and plotting them on a chart.

For this lab, we are using [PubNub](https://www.pubnub.com/) as the MQTT broker.

## Circuit Diagram

Make the following circuit on a solderless breadboard, or on an [Arduino Uno Proto Shield](https://store.arduino.cc/usa/proto-shield-rev3-uno-size):

![Lab 3 Circuit Diagram](https://maxmiaggi.github.io/johnnyfive-iot-class-masters2017/Images/lab3_circuit.png)

## Server-side Node.js

On your server (the host computer), you'll need to write a JavaScript code using Johnny Five library to perform the following functions:

- Reads data from the three sensors connected to the Arduino
- Publishes data to PubNub's data stream network

The following code shows different ways to read and publish data using JavaScript.

**File Name: lab3-server.js**

```javascript
// Initialize PubNub
var PubNub = require("pubnub");
var pubnub = new PubNub({
    ssl          : true,  // <- enable TLS Tunneling over TCP
    publishKey   : "pub-c-43404d0c-...",  // use your own keys
    subscribeKey : "sub-c-486be1d0-..."
});

// Code to publish tempsensor and photoresistor values
var temp = 0;
var light = 0;
var channel = 'temperature-photoresistor';

function publish() {
  var data = { eon: {      // Use "eon" keyword to display the data as chart
    'temperature': temp,
    'light': light
  }};
  pubnub.publish({         // Actual publish()
    channel: channel,
    message: data,
  });
}


// Initialize Johnny-Five
var five = require('johnny-five');

// Specify COM port if J5 cannot detect it
// five.Board({ port: "COM14" }).on('ready', function() {
five.Board().on('ready', function() {

  // Initialize tempsensor
  var temperature = new five.Thermometer({
    controller: 'MCP9808',
    freq: 250
  });

  // Initialize photoresistor
  var photoresistor = new five.Sensor({
    pin: 'A2',
    freq: 250
  });

  // Initialize potentiometer
  var potentiometer = new five.Sensor({
    pin: 'A0',
    freq: 150
  });

  // Read data from tempsensor
  temperature.on('data', function() {
    console.log(this.celsius + '°C', this.fahrenheit + '°F');
    temp = this.celsius;
  });
  
  // Read data from photoresistor
  photoresistor.on('data', function() {
    console.log('photoresistor: ' + this.value);
    light = this.value;
  });

  // Call publish() function every 1000 ms
  setInterval(publish, 1000);


  // Read data from potentiometer and publish immediately
  potentiometer.on('data', function() {
    // value range 0 - 1023
    console.log(this.value, this.raw);

    var data = { eon: {            // Use "eon" to display data as a chart
      'potentiometer': this.value
    }};
    pubnub.publish({               // Actual publish()
      channel: 'potentiometer',
      message: data
    });
  });
});

```


## Browser-side Code

In the browser-side code, we need to create multiple charts using [Eon](https://www.pubnub.com/developers/eon/) and subscribe to the data from PubNub. The browser-side GUI requires HTML, CSS and JavaScript. You can use [Codepen](https://codepen.io/pen/) to type in the code.

You can use the [EON Chart Builder](https://www.pubnub.com/developers/eon/chart/builder/) to assist you in developing your custom web page.

### HTML Code

The following HTML code calls EON scripts and creates basic structure for the web page.

```html

  <!-- Heading of the web page -->
  <header>
    <h1>Sensor Data from Arduino</h1>
  </header>

  <!-- Call Eon scripts and styling -->
  <script type="text/javascript" src="http://pubnub.github.io/eon/v/eon/1.0.0/eon.js"></script>
  <link type="text/css" rel="stylesheet" href="http://pubnub.github.io/eon/v/eon/1.0.0/eon.css" />


  <!-- Basic structure of the web page -->
  <!-- Three divs for each chart -->
  <div id="chart">
    <div id="light"></div>
    <div id="temp"></div>
    <div id="pot"></div>
  </div>


```

### CSS Code

The following CSS code applies some basic styline rules.

```css
  * { -moz-box-sizing: border-box; -webkit-box-sizing: border-box; box-sizing: border-box; }

  /* Basic html and body styling */
  html, body { margin: 0; padding:0 }
  body {
      font-family: 'HelveticaNeue-Light', 'Helvetica Neue Light', 'Helvetica Neue', 'Roboto Light', 'Segoe UI Web Light', 'Segoe UI Light', 'Segoe UI Web Regular', 'Segoe UI', Helvetica, Arial, sans-serif;
      margin: 1em;
  }

  /* Styling headers */
  h1 {
      font-weight: 200;
  }

  /* General chart styling (in addition to eon.css) */
  #chart {
      position: absolute;
      top: 25%;
      left: 0;
      right: 0;
      display: flex;
      align-items: center;
  }

  /* Styling the basic structure of the web page */
  #temp, #light, #pot {
      flex: 1;
      margin: 20px;
  }
```

### JavaScript Code

The following JS code creates the charts, subscribes to data, and populates the chart with the data.

```javascript

  // Initialize PubNub
  var pubnub = new PubNub({
    subscribeKey : "sub-c-486be1d0-..."   // use your own keys
  });

  // Channel to subscribe to
  var channel = 'temperature-photoresistor';

  // Diaplay chart using EON
  eon.chart({
    channels: [channel],      // Channel to subscribe to
    flow: true,           // Make the chart flow...
    generate: {           // Generates chart
      bindto: '#light',       // Which div the chart should be displayed on?
      data: {
        type: 'spline'        // Chart type
      },
      axis: {           // Customize chart axes
        x: {
          type: 'timeseries',
          tick: {
            format: '%H:%m:%S',
            fit: true
          },
          label: {
            text: 'Light over time'
          }
        },
        y: {
          label: {
            text: 'Lumens',
            position: 'outer-middle'
          },
          tick: {
            format: function (d) {
              var df = Number( d3.format('.2f')(d) );
              return df;
            }
          }
        }
      }
    },
    pubnub: pubnub,
    limit: 10,            // Number of data points
    transform: function(m) {
      return { eon: {
        light: m.eon.light
      }}
    }
  });

  eon.chart({
    channels: [channel],
    flow: true,
    generate: {
      bindto: '#temp',
      data: {
        type: 'spline',
        colors: {
          temperature: 'deeppink'
        }
      },
      axis: {
        x: {
          type: 'timeseries',
          tick: {
            format: '%H:%m:%S',
            fit: true
          },
          label: {
            text: 'Temperature over time',
          }
        },
        y: {
          label: {
            text: 'Celsius',
            position: 'outer-middle'
          },
          tick: {
            format: function (d) {
              var df = Number( d3.format('.2f')(d) );
              return df;
            }
          }
        }
      }
    },
    pubnub: pubnub,
    limit: 30,
    transform: function(m) {
      return { eon: {
        temperature: m.eon.temperature
      }}
    }
  });

  eon.chart({
    channels: ['potentiometer'],
    rate: 10,
    generate: {
      bindto: '#pot',
      data: {
        type:'gauge'
      },
      gauge: {
        label:{
          format: function(value, ratio){
            return value; //returning here the value, not in %
          }
        },
        min: 0,
        max: 1023
      },
      color: {
        pattern: ['#FF0000', '#F6C600', '#60B044'],
        threshold: {
          values: [341, 682]
        }
      }
    },
    pubnub: pubnub
  });
```

### All-in-one Code

If you want to host the web page on your own server, you will need to create a single HTML file and include all your HTML/CSS/JS in that one file, or create separate .html, .css, and .js files and call them from the HTML file. Here's how a single HTML file would look like:

**File Name: lab3-visuals.html**

```html
<!DOCTYPE html>
<html lang='en'>
<head>
  <meta charset='UTF-8'>
  <title>Sensor Data from Arduino</title>
  <script type="text/javascript" src="http://pubnub.github.io/eon/v/eon/1.0.0/eon.js"></script>
  <link type="text/css" rel="stylesheet" href="http://pubnub.github.io/eon/v/eon/1.0.0/eon.css" />
  <style type='text/css'>
    * { -moz-box-sizing: border-box; -webkit-box-sizing: border-box; box-sizing: border-box; }
    html, body { margin: 0; padding:0 }
    body {
        font-family: 'HelveticaNeue-Light', 'Helvetica Neue Light', 'Helvetica Neue', 'Roboto Light', 'Segoe UI Web Light', 'Segoe UI Light', 'Segoe UI Web Regular', 'Segoe UI', Helvetica, Arial, sans-serif;
        margin: 1em;
      }
      h1 {
      font-weight: 200;
      }
    #chart {
      position: absolute;
      top: 25%;
      left: 0;
      right: 0;
      display: flex;
      align-items: center;
    }
    #temp, #light, #pot {
      flex: 1;
      margin: 20px;
    }
  </style>
</head>
<body>

<header>
  <h1>Sensor Data from Arduino</h1>
</header>

<div id="chart">
  <div id="light"></div>
  <div id="temp"></div>
  <div id="pot"></div>
</div>

<script type='text/javascript'>
  var pubnub = new PubNub({
    subscribeKey : "sub-c-486be1d0-5c63-11e7-b272-02ee2ddab7fe"
  });

  var channel = 'temperature-photoresistor';

  eon.chart({
    channels: [channel],
    flow: true,
    generate: {
      bindto: '#light',
      data: {
        type: 'spline'
      },
      axis: {
        x: {
          type: 'timeseries',
          tick: {
            format: '%H:%m:%S',
            fit: true
          },
          label: {
            text: 'Light over time'
          }
        },
        y: {
          label: {
            text: 'Lumens',
            position: 'outer-middle'
          },
          tick: {
            format: function (d) {
              var df = Number( d3.format('.2f')(d) );
              return df;
            }
          }
        }
      }
    },
    pubnub: pubnub,
    limit: 10,
    transform: function(m) {
      return { eon: {
        light: m.eon.light
      }}
    }
  });

  eon.chart({
    channels: [channel],
    flow: true,
    generate: {
      bindto: '#temp',
      data: {
        type: 'spline',
        colors: {
          temperature: 'deeppink'
        }
      },
      axis: {
        x: {
          type: 'timeseries',
          tick: {
            format: '%H:%m:%S',
            fit: true
          },
          label: {
            text: 'Temperature over time',
          }
        },
        y: {
          label: {
            text: 'Celsius',
            position: 'outer-middle'
          },
          tick: {
            format: function (d) {
              var df = Number( d3.format('.2f')(d) );
              return df;
            }
          }
        }
      }
    },
    pubnub: pubnub,
    limit: 30,
    transform: function(m) {
      return { eon: {
        temperature: m.eon.temperature
      }}
    }
  });

  eon.chart({
    channels: ['potentiometer'],
    rate: 10,
    generate: {
      bindto: '#pot',
      data: {
        type:'gauge'
      },
      gauge: {
        label:{
          format: function(value, ratio){
            return value; //returning here the value, not in %
          }
        },
        min: 0,
        max: 1023
      },
      color: {
        pattern: ['#FF0000', '#F6C600', '#60B044'],
        threshold: {
          values: [341, 682]
        }
      }
    },
    pubnub: pubnub
  });
</script>
</body>
</html>

```

## Screenshot


![Lab 3 Screenshot](https://maxmiaggi.github.io/johnnyfive-iot-class-masters2017/Images/lab3_screenshot_1.png)

## Working

Arduino will read the data from the three sensors based on the commands from Johnny Five. The Node.js code will then publish the data to PubNub's real-time data stream network. The remote computer on the other side will read the data and display them beautifully on a chart based on the HTML/CSS/JS code.

















































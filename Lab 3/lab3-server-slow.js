var PubNub = require("pubnub");
var pubnub = new PubNub({
    ssl          : true,  // <- enable TLS Tunneling over TCP
    publishKey   : "pub-c-43404d0c-78e6-4ebb-a9e7-ca0e54125a52",
    subscribeKey : "sub-c-486be1d0-5c63-11e7-b272-02ee2ddab7fe"
});

var temp = 0;
var light = 0;
var channel = 'temperature-photoresistor';


function publish() {
  var data = { eon: {
    'temperature': temp,
    'light': light
  }};
  pubnub.publish({
    channel: channel,
    message: data,
  });
}


// Johnny-Five
// Using a temperature sensor, type DS18B20 requires OneWire support using the ConfigurableFirmata

var five = require('johnny-five');

five.Board({ port: "COM14" }).on('ready', function() {
//five.Board().on('ready', function() {


  var temperature = new five.Thermometer({
    controller: 'MCP9808',
    freq: 1000
  });

  var photoresistor = new five.Sensor({
    pin: 'A2',
    freq: 1000
  });


  var potentiometer = new five.Sensor({
    pin: 'A0',
    freq: 1000
  });


  temperature.on('data', function() {
    console.log(this.celsius + '°C', this.fahrenheit + '°F');
    temp = this.celsius;
  });

  photoresistor.on('data', function() {
    console.log('photoresistor: ' + this.value);
    light = this.value;
  });

  setInterval(publish, 1000);



  potentiometer.on('data', function() {
    // value range 0 - 1023
    console.log(this.value, this.raw);

    var data = { eon: {
      'potentiometer': this.value
    }};
    pubnub.publish({
      channel: 'potentiometer',
      message: data
    });
  });
});

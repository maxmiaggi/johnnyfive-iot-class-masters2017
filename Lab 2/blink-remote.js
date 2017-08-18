var five = require('johnny-five');
// var board = new five.Board({ port: "COM14" });
var board = new five.Board();

var PubNub = require("pubnub");
var pubnub = new PubNub({
    ssl          : true,  // <- enable TLS Tunneling over TCP
    publishKey   : "pub-c-cfd2c879-04ee-40ad-bf7f-6d4df977ff7a",
    subscribeKey : "sub-c-801ccfbc-41a2-11e7-bc55-0619f8945a4f"
});

var channel = 'led';
 
board.on('ready', function() {
  var led = new five.Led(13); // pin 13

  pubnub.addListener({
    message: function(m) {
      if(m.message.blink == true) {
        led.blink(500);
      } else {
        led.stop();
        led.off();
      }
    }
  });

  pubnub.subscribe({
    channels: [channel],
  });


});

var five = require('johnny-five');

five.Board({ port: "COM14" }).on('ready', function() {
//five.Board().on('ready', function() {

  var thermometer = new five.Thermometer({
    controller: "MCP9808"
  });

  thermometer.on("change", function() {
    console.log("Thermometer");
    console.log("  celsius      : ", this.celsius);
    console.log("  fahrenheit   : ", this.fahrenheit);
    console.log("  kelvin       : ", this.kelvin);
    console.log("--------------------------------------");
  });
});

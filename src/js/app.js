var OWMWeather = require('lib/owm_weather.js');

var owmWeather = new OWMWeather({ baseAppKey: 10 });

Pebble.addEventListener('ready', function(e) {
  console.log('PebbleKit JS ready!');
  Pebble.sendAppMessage({ 'AppKeyJSReady': 1 });
});

Pebble.addEventListener('appmessage', function(e) {
  console.log('appmessage: ' + JSON.stringify(e.payload));
  owmWeather.appMessageHandler(e);
});

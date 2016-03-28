
// i guess Pebble just magicks in from somewhere.
Pebble.addEventListener('ready', function()
{
  var lastLat = 361;
  var lastLon = 361;

  // immediately start requesting geolocation.
  // TODO: wait until we actually need it? battery?
  // TODO: deal with if the user has denied Pebble GPS.
  navigator.geolocation.watchPosition(function(position)
  {
    if (position.coords.latitude === lastLat && position.coords.longitude === lastLon) return;
    lastLat = position.coords.latitude;
    lastLon = position.coords.longitude;

    Pebble.sendAppMessage({
      lat: position.coords.latitude.toString(), // can't send floats
      lon: position.coords.longitude.toString()
    });
  },
  {
    enableHighAccuracy: true,
    maximumAge: 2 * 60 * 1000 // 2 minutes ago is okay
  });
});


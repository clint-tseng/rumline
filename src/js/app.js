
// i guess Pebble just magicks in from somewhere.
Pebble.addEventListener('ready', function()
{
  // immediately start requesting geolocation.
  // TODO: wait until we actually need it? battery?
  // TODO: deal with if the user has denied Pebble GPS.
  navigator.geolocation.watchPosition(function(position)
  {
    Pebble.sendMessage({
      lat: position.coords.latitude.toString(), // can't send floats
      lon: position.coords.longitude.toString()
    }
  },
  {
    enableHighAccuracy: true,
    maximumAge: 2 * 60 * 1000 // 2 minutes ago is okay
  });
});



// UTIL/MATH
var earthMeters = 6371000;
var nm = 1852;

var toRadians = function(deg) { return deg / 180 * Math.PI; };
var toDegrees = function(rad) { return rad * 180 / Math.PI; };
var square = function(x) { return x * x; };

var distance = function(fromLat, fromLon, toLat, toLon)
{
  var phiFrom = toRadians(fromLat);
  var phiTo = toRadians(toLat);
  var deltaPhi = toRadians(toLat - fromLat);
  var deltaLambda = toRadians(toLon - fromLon);

  var a = square(Math.sin(deltaPhi / 2)) +
          Math.cos(phiFrom) * Math.cos(phiTo) * square(Math.sin(deltaLambda / 2));
  var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
  var d = earthMeters * c;

  return d / nm;
};

var bearing = function(fromLat, fromLon, toLat, toLon)
{
  var phiFrom = toRadians(fromLat);
  var phiTo = toRadians(toLat);
  var deltaLambda = toRadians(toLon - fromLon);

  var y = Math.sin(deltaLambda) * Math.cos(phiTo);
  var x = Math.cos(phiFrom) * Math.sin(phiTo) -
          Math.sin(phiFrom) * Math.cos(phiTo) * Math.cos(deltaLambda);

  return (toDegrees(Math.atan2(y, x)) + 360) % 360;
};


// BUSINESS

var lastLat = 181, lastLon = 181;
var tgtLat = 181, tgtLon = 181;

var maybeUpdate = function()
{
  if (lastLat > 180 || lastLon > 180 || tgtLat > 180 || tgtLon > 180) return;

  var bear = bearing(lastLat, lastLon, tgtLat, tgtLon);
  var dist = distance(lastLat, lastLon, tgtLat, tgtLon);

  if (dist >= 100)
    dist = Math.round(dist);
  else
    dist = Math.round(dist * 100) / 100;

  Pebble.sendAppMessage({
    bearing: Math.round(bear).toString(),
    distance: dist.toString()
  });
};


// PEBBLE THINGS

// i guess Pebble just magicks in from somewhere.
Pebble.addEventListener('ready', function()
{
  var geolocated = false;
  var maybeGeolocate = function()
  {
    if (geolocated === true) return;
    geolocated = true;

    // start requesting geolocation.
    // TODO: deal with if the user has denied Pebble GPS.
    navigator.geolocation.watchPosition(function(position)
    {
      if (position.coords.latitude === lastLat && position.coords.longitude === lastLon) return;
      lastLat = position.coords.latitude;
      lastLon = position.coords.longitude;

      maybeUpdate();
    },
    {
      enableHighAccuracy: true,
      maximumAge: 2 * 60 * 1000 // 2 minutes ago is okay
    });
  };

  // also start listening for marks from the phone.
  Pebble.addEventListener('appmessage', function(event)
  {
    tgtLat = parseFloat(event.payload.lat);
    tgtLon = parseFloat(event.payload.lon);

    maybeGeolocate();
    maybeUpdate();
  });
});

Pebble.addEventListener('showConfiguration', function()
{
  var url = 'http://to-mark.giantacorn.com';
  Pebble.openURL(url);
});


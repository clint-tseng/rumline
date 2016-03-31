
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
var tgtId = -1;

var maybeUpdate = function()
{
  if (lastLat > 180 || lastLon > 180 || tgtId < 0) return;

  var mark = findMark(tgtId);
  if (mark === null) return;

  var tgtLat = mark.lat, tgtLon = mark.lon;

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
    tgtId = parseFloat(event.payload.id);

    maybeGeolocate();
    maybeUpdate();
  });
});


// CONFIG THINGS

var currentConf;
try { currentConf = JSON.parse(localStorage.getItem('tomark')); } catch (_) { currentConf = { groups: [] }; }

var baseUrl = 'http://to-mark.giantacorn.com/';
Pebble.addEventListener('showConfiguration', function()
{
  var url;
  var conf = localStorage.getItem('tomark');
  try { JSON.parse(conf); } catch (_) { conf = null; }

  if (conf == null)
  {
    Pebble.openURL(baseUrl);
  }
  else
  {
    var id = 'p-' + Pebble.getWatchToken();

    var req = new XMLHttpRequest();
    req.setRequestHeader('Content-Type', 'text/plain');
    req.open('POST', baseUrl + 'config/' + id);
    req.send(conf);

    Pebble.openURL(baseUrl + '#' + id);
  }
});

Pebble.addEventListener('webviewclosed', function(event)
{
  if ((event.response == null) || (event.response == '')) return;

  var req = new XMLHttpRequest();
  req.onload = function()
  {
    localStorage.setItem('tomark', this.responseText);
    currentConf = JSON.parse(this.responseText);
    sendSettingsToPebble(currentConf, 3, function()
    {
      Pebble.showSimpleNotificationOnPebble('To Mark', 'Settings saved!');
    });
  };
  req.open('GET', baseUrl + 'config/' + event.response);
  req.send();
});

var cfgMessages = { start: 0, group: 1, mark: 2, end: 3 };
var sendSettingsToPebble = function(conf, attempts, callback)
{
  if (attempts === 0) return Pebble.showSimpleNotificationOnPebble('To Mark', 'Failed to save settings. Check Bluetooth and try again? :(');
  var fail = function() { sendSettingsToPebble(conf, attempts - 1, callback); };

  // so, this is sort of done serially as a weirdo state machine
  // because we just have a lot of data to push.
  // ?? do we need to wait for pebble ready here?
  Pebble.sendAppMessage({ cfgMessage: cfgMessages.start });

  var i = -1;
  var sendGroup = function()
  {
    i++;
    if (i >= conf.groups.length)
    {
      Pebble.sendAppMessage({ cfgMessage: cfgMessages.end });
      return callback();
    }

    var group = conf.groups[i];

    var j = -1;
    var sendMark = function()
    {
      j++;
      if (j >= group.marks.length) return sendGroup();

      var mark = group.marks[j];
      Pebble.sendAppMessage({ cfgMessage: cfgMessages.mark, cfgMarkId: mark.id, cfgMarkName: mark.name }, sendMark, fail);
    };

    Pebble.sendAppMessage({ cfgMessage: cfgMessages.group, cfgGroupName: group.name }, sendMark, fail);
  };
  sendGroup();
};

var findMark = function(id)
{
  var groups = currentConf.groups;
  for (var i = 0; i < groups.length; i++)
    for (var j = 0; j < groups[i].marks.length; j++)
    {
      var mark = groups[i].marks[j];
      if (mark.id === id) return mark;
    }
  return null;
};


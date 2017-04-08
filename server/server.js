var express = require('express');
var bodyParser = require('body-parser');
var https = require('https');

var server = express();
var data = require('./data');

var fetchThrough = function(url, stream)
{
  https.get(url, function(remote)
  {
    remote.pipe(stream);
    remote.on('end', function() { stream.end(); });
  }).on('error', function() { stream.end(); });
};

server.use(bodyParser.text());

var ok = function(res) { res.status(200).type('json').send({ success: true }); };

server.get('/config/:guid', function(req, res)
{
  data.get(req.params.guid).then(function(body)
  {
    if (body) res.status(200).type('json').send(body);
    else res.status(404).type('json').send({ success: false });
  });
});

server.post('/config/:guid', function(req, res)
{
  data.put(req.params.guid, req.body).then(function(result)
  {
    if (result === true) ok(res);
    else res.status(400).type('json').send({ success: false });
  });
});

server.get('/fetch/caltopo/:id', function(req, res)
{
  fetchThrough('https://caltopo.com/m/' + req.params.id + '?format=json', res);
});

server.post('/prune', function(req, res)
{
  data.prune().then(function() { ok(res); });
});

server.use(express.static(__dirname + '/web/'));
server.listen(4400);


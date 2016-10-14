# to mark

## building the client

Just a standard `pebble build`. There is also a `make test` that sort of exercises some C code but it's nothing comprehensive.

## building config

The config page styles are a sass file at `server/web/app.sass` that needs to be built to css if you make changes.

## running the server

The server is necessary because our config payloads can easily be too long to be URL-coded. So it's just a k/v store.

To build and run, just do the usual `npm install` followed by `node server.js`. It runs on port 4400 because why not?

No migrations are provided because it's so dead simple.

    create user tomark;
    create database tomark with owner=tomark encoding='UTF8';
    \c tomark
    create table configs (guid varchar(40), data text, at timestamp);
    alter table configs owner to tomark;

## license

[WTFPL](http://www.wtfpl.net/about/)


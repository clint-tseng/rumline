var knex = require('knex');

var db = knex({
  client: 'pg',
  connection: {
    host: 'localhost',
    user: 'tomark',
    database: 'tomark'
  }
});

module.exports = {
  put: function(guid, data)
  {
    var now = new Date();
    return db.insert({ guid: guid, data: data, at: now })
             .into('configs')
             .then(function(result) { return result.rowCount === 1; });
  },
  get: function(guid)
  {
    return db.select('data')
             .from('configs')
             .where({ guid: guid })
             .orderBy('at', 'desc')
             .limit(1)
             .then(function(result)
             {
               return (result[0] != null) ? result[0].data : {};
             });
  },
  prune: function()
  {
    var cutoff = new Date();
    cutoff.setMinutes(cutoff.getMinutes() - 5)
    return db.del().from('configs').where('at', '<', cutoff);
  }
};


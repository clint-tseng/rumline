;(function($){$(function() {

// UTIL
var remove = function(array, elem)
{
  var idx = array.indexOf(elem);
  if (idx >= 0) array.splice(idx, 1);
};

// UI
// groups
var newGroup = function(group)
{
  if (group == null) groups.push(group = { marks: [] });

  var groupElem = $('#templates .group').clone();
  $('.groups').append(groupElem);
  groupElem.data('group', group);

  groupElem.find('.group-name').val(group.name);
};
$('.add-group').on('click', function() { newGroup(); });
$(document).on('click', '.delete-group', function()
{
  var groupElem = $(this).closest('.group');
  remove(groups, groupElem.data('group'));
  groupElem.slideUp(function() { groupElem.remove(); });
});

$(document).on('input', '.group input.group-name', function()
{
  var input = $(this);
  input.closest('.group').data('group').name = input.val();
});

// marks
var newMark = function(group, mark)
{
  if (mark == null) group.marks.push(mark = { id: ++data.uid });

  var groupElem = $('.group').filter(function() { return $(this).data('group') === group; });
  var markElem = $('#templates .mark').clone();
  groupElem.find('.marks').append(markElem);
  markElem.data('mark', mark);

  for (var k in mark) markElem.find('.mark-' + k).val(mark[k]);
  updateDMS(mark, 'lat');
  updateDMS(mark, 'lon');
};
$(document).on('click', '.add-mark', function() { newMark($(this).closest('.group').data('group')); });
$(document).on('click', '.delete-mark', function()
{
  var markElem = $(this).closest('.mark');
  remove(markElem.closest('.group').data('group').marks, markElem.data('mark'));
  markElem.slideUp(function() { markElem.remove(); });
});

$(document).on('click', '.to-dms', function() { $(this).closest('.decimal').addClass('hide').next().removeClass('hide'); });
$(document).on('click', '.to-decimal', function() { $(this).closest('.dms').addClass('hide').prev().removeClass('hide'); });

$(document).on('input', '.mark input', function()
{
  var input = $(this);
  var mark = input.closest('.mark').data('mark');

  if (input.closest('.dms').length > 0)
  {
    var latLon = (input.closest('.dms-lat').length > 0) ? 'lat' : 'lon';
    var line = input.closest('.line');
    var d = parseFloat(line.find('.dms-d').val()) || 0;
    var m = parseFloat(line.find('.dms-m').val()) || 0;
    var s = parseFloat(line.find('.dms-s').val()) || 0;
    var result = d + (m / 60) + (s / 3600);

    mark[latLon] = result;
    input.closest('.mark').find('.mark-' + latLon).val(result);
  }
  else
  {
    var field = input.attr('class').match(/^mark-(.*)$/)[1];
    if (input.closest('.decimal').length > 0)
    {
      mark[field] = parseFloat(input.val()) || 0;
      updateDMS(mark, field);
    }
    else
    {
      mark[field] = input.val();
    }
  }
});

var updateDMS = function(mark, field)
{
  var markElem = $('.mark').filter(function() { return $(this).data('mark') === mark; });
  var container = markElem.find('.dms-' + field);

  var value = mark[field] || 0;
  container.find('.dms-d').val(Math.floor(value));

  value = (value - Math.floor(value)) * 60;
  container.find('.dms-m').val(Math.floor(value));

  value = (value - Math.floor(value)) * 60;
  container.find('.dms-s').val(Math.round(value * 1000) / 1000);
};


// DATA MODEL
// load data
var data = { uid: 0, groups: [{ name: "My first folder", marks: [{ id: 0, name: "My first mark" }] }] };
if (window.location.hash !== '') data = JSON.parse(decodeURIComponent(window.location.hash.slice(1)));
var groups = data.groups;

// render elems
for (var i in groups)
{
  newGroup(groups[i]);
  for (var j in groups[i].marks) newMark(groups[i], groups[i].marks[j]);
}

// on save
$('#save').on('click', function() { window.location = 'pebblejs://close#' + encodeURIComponent(JSON.stringify(data)); });


// DEBUG
window.groups = groups;

}); })(jQuery);


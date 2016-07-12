var test = require('tap').test;
var Obj = require('bindings')('addon');
var heapdump = require('heapdump');

var obj = new Obj();

function fn() {
  var callback = obj.create();
  console.log(callback());
}

test('memory', function(t) {
    var i = 0;
    var max = 10000;
    var memoryMax = 15 * 1024 * 1024;
    var heapGrowth = 0;
    var lastHeapSize = 0;

    var interval = setInterval(function () {
        fn();

        if (i % (max / 10) == 0) {
            gc();

            var currentHeapSize = process.memoryUsage()['heapUsed'];

            if (lastHeapSize == 0) {
                lastHeapSize = currentHeapSize;
            }

            heapGrowth = heapGrowth + currentHeapSize - lastHeapSize;
            lastHeapSize = currentHeapSize;

            // heapdump.writeSnapshot();
        }

        if (++i == max) {
            t.ok(heapGrowth < memoryMax, heapGrowth);
            t.end();

            clearInterval(interval);
        }
    }, 1);
});

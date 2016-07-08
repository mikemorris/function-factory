var test = require('tap').test;
var addon = require('bindings')('addon');
var heapdump = require('heapdump');

function fn() {
  new addon().create()();
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
            if (gc) gc();

            var currentHeapSize = process.memoryUsage()['heapUsed'];

            // Print some progress, so slow build bots don't timeout.
            t.comment(i);

            if (lastHeapSize == 0) {
                lastHeapSize = currentHeapSize;
            }

            heapGrowth = heapGrowth + currentHeapSize - lastHeapSize;
            lastHeapSize = currentHeapSize;

            heapdump.writeSnapshot();
        }

        if (++i == max) {
            t.ok(heapGrowth < memoryMax, heapGrowth);
            t.end();

            clearInterval(interval);
        }
    }, 1);
});

var platform = cordova.require('cordova/platform');
var exec = cordova.require('cordova/exec');

exports.write = function(socketId, data, callback) {
    var type = Object.prototype.toString.call(data).slice(8, -1);
    if (type != 'ArrayBuffer') {
        throw new Error('chrome.socket.write - data is not an ArrayBuffer! (Got: ' + type + ')');
    }
    var win = callback && function(bytesWritten) {
        var writeInfo = {
            bytesWritten: bytesWritten
        };
        callback(writeInfo);
    };
    var fail = callback && function() {
        var writeInfo = {
            bytesWritten: 0
        };
        callback(writeInfo);
    };
data = String.fromCharCode.apply(null, new Uint8Array(data));
    exec(win, fail, 'ChromeSocket', 'write', [socketId, data]);
};

Duktape.modSearch = function (id, require, exports, module) {
    var res;

    print('loading module:', id, './' + id + '.js');

    res = libdawn.readFile('./' + id + '.js');
    if (typeof res === 'string') {
        print(res);
        return res;
    }

    throw new Error('module not found: ' + id);
};

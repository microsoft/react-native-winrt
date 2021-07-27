
const { Windows } = require('./build/Debug/winrt')

exports.listPackages = function() {
    var mgr = new Windows.Management.Deployment.PackageManager();
    var pkgs = mgr.findPackages();
    for (var i = 0; i < pkgs.length; ++i) {
        var pkg = pkgs.getAt(i);
        try {
            if (!pkg.isFramework)
            {
                console.log(pkg.displayName);
            }
        } catch {}
    }
}

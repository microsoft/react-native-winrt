
module.exports = {
  dependency: {
    platforms: {
      windows: {
        sourceDir: 'windows',
        projects: [
          {
            projectFile: 'WinRTTurboModule/WinRTTurboModule.vcxproj',
            directDependency: true
          }
        ]
      }
    }
  }
};

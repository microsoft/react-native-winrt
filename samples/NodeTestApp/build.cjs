const { execSync, exec } = require('child_process');
const path = require('path');
const fs = require('fs');
const { config } = require('process');
const rootDir = __dirname;
const nugetPkgDir = path.join(rootDir, 'int/nuget-packages');
const intInc = path.join(rootDir, 'int/inc');

// TODO: Read from environment variable or something?
const configuration='release';
const platform='x64';

function getPackagePath(packageName) {
  
  // Find the first directory that matches the package name
  for (const dir of fs.readdirSync(nugetPkgDir)) {
    // print
    console.log(`dir: ${dir}`);
    if (dir.startsWith(packageName)) {
      return path.join(nugetPkgDir, dir);
    }
  }

  throw new Error(`Package not found: ${packageName}`);
}

// Helper function to execute commands
function runCommand(command, cwd = process.cwd()) {
  console.log(`Running: ${command} in ${cwd}`);
  try {
    execSync(command, { cwd, stdio: 'inherit' });
  } catch (error) {
    console.error(`Command failed: ${command}`);
    process.exit(1);
  }
}

function downloadNuget() {
  const https = require('https');
  const http = require('http');
  const toolsDir = path.join(rootDir, 'tools');
  const nugetPath = path.join(toolsDir, 'nuget.exe');
  
  // Create tools directory if it doesn't exist
  if (!fs.existsSync(toolsDir)) {
    fs.mkdirSync(toolsDir, { recursive: true });
  }
  
  if (!fs.existsSync(nugetPath)) {
    console.log('Downloading NuGet.exe...');
    const url = 'https://dist.nuget.org/win-x86-commandline/latest/nuget.exe';
    
    // Use execSync to download with curl (which is available on most systems)
    try {
      execSync(`curl -o "${nugetPath}" "${url}"`, { stdio: 'inherit' });
      console.log('NuGet.exe downloaded successfully');
    } catch (error) {
      console.error(`Failed to download NuGet.exe: ${error.message}`);
      process.exit(1);
    }
  } else {
    console.log('NuGet.exe already exists, skipping download.');
  }
  
  return nugetPath;
}

function nugetRestore() {
  const nugetPath = downloadNuget();
  runCommand(`"${nugetPath}" restore -PackagesDirectory ${nugetPkgDir}`);
}

function runMsBuild(projectPath, arguments = '') {
  // Use vswhere to find the MSBuild path
  const programFilesPath = process.env['ProgramFiles(x86)'];
  const vsWherePath = path.join(programFilesPath, 'Microsoft Visual Studio\\Installer\\vsWhere.exe');
  
  try {
    // Find the latest Visual Studio installation with MSBuild
    const vsPathCommand = `"${vsWherePath}" -latest -products * -requires Microsoft.Component.MSBuild -property installationPath`;
    const vsPath = execSync(vsPathCommand).toString().trim();
    
    // Path to MSBuild.exe (the version may vary depending on installed VS)
    const msBuildPath = path.join(vsPath, 'MSBuild', 'Current', 'Bin', 'MSBuild.exe');
    
    // Verify MSBuild.exe exists
    if (!fs.existsSync(msBuildPath)) {
      throw new Error(`MSBuild not found at ${msBuildPath}`);
    }
    
    // Build the project
    const msbuildCommand = `"${msBuildPath}" "${projectPath}" /p:Configuration=${configuration},Platform=${platform} ${arguments}`;
    runCommand(msbuildCommand);
  } catch (error) {
    console.error(`Failed to run MSBuild: ${error.message}`);
    process.exit(1);
  }
}

function runCppWinrt() {
  const cppwinrtPath = path.join(getPackagePath('Microsoft.Windows.CppWinRT'), 'bin', 'cppwinrt.exe');

  // Setup paths for cppwinrt
  const winAppSdkDir = getPackagePath('Microsoft.WindowsAppSDK');
  const winPlatSdkDir = getPackagePath('Microsoft.Windows.SDK.CPP');
  const foundationDir = getPackagePath('Microsoft.WindowsAppSDK.Foundation');
  
  // Input paths
  /*
  const input = [
    path.join(winAppSdkDir, 'lib', 'uap10.0.18362'),
    path.join(winPlatSdkDir, 'c', 'References', '10.0.26100.0')
  ].join(' ');
  */
  const input = [
    path.join(foundationDir, 'metadata'),
    path.join(getPackagePath('Microsoft.WindowsAppSDK.InteractiveExperiences'), 'metadata', '10.0.18362.0'),
    path.join(getPackagePath('Microsoft.WindowsAppSDK.AI'), 'metadata'),
    '..\\..\\tests\\RnWinRTTests\\windows\\TestComponent\\x64\\Debug\\TestComponent\\TestComponent.winmd'
  ].join(' ');
  
  //const reference = path.join(winAppSdkDir, 'lib');
  const reference = 'c:\\windows\\system32\\winmetadata';

  console.log('Calling cppwinrt.exe...');  
  runCommand(`"${cppwinrtPath}" -input ${input} -reference ${reference} -output ${intInc}`);
  //runCommand(`"${cppwinrtPath}" -input ${input} -output ${intInc}`);

  console.log('Copying DLLs...');
  const winAppSdkFoundationRuntimeDir = path.join(foundationDir, 'runtimes', 'win-x64', 'native');
  const winAppSdkFoundationDll = path.join(winAppSdkFoundationRuntimeDir, 'Microsoft.WindowsAppRuntime.Bootstrap.dll');
  //fs.copyFileSync(winAppSdkFoundationDll, '.');
  execSync(`copy /y "${winAppSdkFoundationDll}" "${rootDir}"`, { stdio: 'inherit' });

}

function generateProps() {
  const gypiPath = path.join(rootDir, 'native\\build\\config.gypi');

  // First, strip leading # from the file
  const gypiContents = fs.readFileSync(gypiPath, 'utf8');
  const strippedContents = gypiContents.replace(/^#.*\n/gm, '');

  // Read as JSON
  const gypiJson = JSON.parse(strippedContents);

  const nodedir = gypiJson.variables.nodedir;
  console.log(`nodedir: ${nodedir}`);

  // Write the generated.props file
  const generatedPropsPath = path.join(rootDir, 'int\\generated.props');
  const generatedProps = `<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <PropertyGroup Label="Globals">
    <NodeDir>${nodedir}</NodeDir>
  </PropertyGroup>
</Project>`;

  // First, ensure the int directory exists
  const intDir = path.join(rootDir, 'int');
  if (!fs.existsSync(intDir)) {
    fs.mkdirSync(intDir);
  }

  fs.writeFileSync(generatedPropsPath, generatedProps);
}

// Define build tasks
const tasks = {
  configure() {
    const nativeDir = path.join(rootDir, 'native');
    runCommand('node-gyp clean', nativeDir);
    runCommand('node-gyp configure', nativeDir);
    generateProps();
    nugetRestore();
    runCppWinrt();
  },
  
  build() {
    runMsBuild('WinUIHelper\\WinUIHelper.sln', '/bl');

    const buildDir = path.join(rootDir, 'build', platform, configuration);

    if (!fs.existsSync(buildDir)) {
      fs.mkdirSync(buildDir, { recursive: true });
    }

    // Copy the electron exe to the build directory
    const electronDir = path.join(rootDir, 'node_modules', 'electron', 'dist');
    console.log(`Copying ${electronDir} to ${buildDir}...`);
    execSync(`xcopy /eiyd "${electronDir}" "${buildDir}"`, { stdio: 'inherit' });

    // Build the native addon
    runCommand('node-gyp build', path.join(rootDir, 'native'));

    // Copy the native addon to the build directory
    //const nativeFile = path.join(rootDir, 'native', 'build', 'Release', 'native.node');
    //console.log(`Copying file ${nativeFile} to ${buildDir}...`);
    //fs.copyFileSync(nativeFile, path.join(buildDir, 'native.node'));
    
    console.log('Build complete!');
  },
  
  rebuild() {
    this.configure();
    runMsBuild('WinUIHelper\\WinUIHelper.sln', '/t:clean');
    this.build();
  },

  downloadNuget() {
    const nugetPath = downloadNuget();
    console.log(`NuGet.exe is located at: ${nugetPath}`);
  },

  nugetRestore() {
    nugetRestore();
  },

  runCppWinrt() {
    runCppWinrt();
  }
};

// Parse command line arguments
const command = process.argv[2];

if (tasks[command]) {
  tasks[command]();
} else {
  console.log('Available commands: configure, build, rebuild');
  process.exit(1);
}
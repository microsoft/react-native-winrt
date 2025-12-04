const { execSync } = require('child_process');
const path = require('path');
const fs = require('fs');

function run(command, options = {}) {
  console.log(`Running: ${command}`);
  try {
    execSync(command, { stdio: 'inherit', ...options });
  } catch (error) {
    console.error('Failed!');
    process.exit(1);
  }
}

function runAndCapture(command) {
  try {
    return execSync(command, { encoding: 'utf8' }).trim();
  } catch (error) {
    console.error(`Failed to execute: ${command}`);
    console.error(error.message);
    process.exit(1);
  }
}

console.log('=== Running rnwinrt.exe ===');

// Get the winapp path and construct packages directory
const winappPath = runAndCapture('npx winapp get-winapp-path --global');
const pkgPath = path.join(winappPath, 'packages');

console.log(`Using packages directory: ${pkgPath}`);

const rnwinrt = path.resolve('..\\..\\rnwinrt\\x64\\Debug\\rnwinrt.exe');

// Helper function to find package directory by prefix
function findPackageDir(prefix) {
  const packages = fs.readdirSync(pkgPath);
  const found = packages.find(pkg => pkg.startsWith(prefix));
  if (!found) {
    console.error(`Could not find package matching: ${prefix}`);
    process.exit(1);
  }
  console.log(`Found package: ${found}`);
  return path.join(pkgPath, found);
}

// Find package directories dynamically
const foundationPkg = findPackageDir('Microsoft.WindowsAppSDK.Foundation.');
const interactiveExpPkg = findPackageDir('Microsoft.WindowsAppSDK.InteractiveExperiences.');
const aiPkg = findPackageDir('Microsoft.WindowsAppSDK.AI.');

// Build input paths
const input = [
  path.join(foundationPkg, 'metadata'),
  path.join(interactiveExpPkg, 'metadata', '10.0.18362.0'),
  path.join(aiPkg, 'metadata'),
  'c:\\windows\\system32\\winmetadata'
].join(' ');

// Include namespaces
const include = [
  'Microsoft.Windows.ApplicationModel.WindowsAppRuntime',
  'Microsoft.UI.Windowing',
  'Microsoft.UI.Dispatching',
  'Windows.Graphics',
  'Microsoft.UI',
  'Microsoft.Windows.AI.Text',
  'Microsoft.Windows.AI.ContentSafety'
].join(' ');

// Exclude namespaces
const exclude = [
  'Windows.Graphics.DirectX',
  'Windows.Graphics.Printing',
  'Windows.Graphics.Display',
  'Windows.Graphics.Printing.Workflow',
  'Windows.Graphics.Capture',
  'Windows.Graphics.Holographic',
  'Windows.Graphics.Imaging',
  'Microsoft.UI.Composition',
  'Microsoft.UI.Content',
  'Microsoft.UI.Input',
  'Microsoft.UI.System'
].join(' ');

const generatedDir = path.resolve('int', 'generated');

// Ensure the generated directory exists and is empty
if (fs.existsSync(generatedDir)) {
  console.log(`Removing existing directory: ${generatedDir}`);
  fs.rmSync(generatedDir, { recursive: true, force: true });
}

console.log(`Creating directory: ${generatedDir}`);
fs.mkdirSync(generatedDir, { recursive: true });

// Run rnwinrt
const rnwinrtCmd = `"${rnwinrt}" -node -input ${input} -output "${generatedDir}" -tsoutput "${path.join(generatedDir, 'ts')}" -include ${include} -exclude ${exclude}`;
console.log(rnwinrtCmd);
run(rnwinrtCmd);

// Copy generated files
const sourcePattern = path.join(generatedDir, 'rnwinrt', '*');
const destDir = path.resolve('winrtaddon', 'generated', 'rnwinrt');

console.log(`Copy from ${sourcePattern} to ${destDir}\\`);
fs.mkdirSync(destDir, { recursive: true });
run(`copy /y "${sourcePattern}" "${destDir}\\"`, { shell: 'cmd.exe' });


console.log('\nSuccess!');

// Outputs newline-separated list of generated .cpp files for gyp 'sources'.
// Relative paths are from the directory containing binding.gyp (nodewinrt/).

const fs = require('fs');
const path = require('path');

const baseDir = path.join(__dirname, '.', 'generated', 'rnwinrt');

function listCppFiles(dir, relRoot) {
  for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
    const abs = path.join(dir, entry.name);
    const rel = path.join(relRoot, entry.name).replace(/\\/g, '/');
    if (entry.isDirectory()) {
      // Recurse (in case future codegen adds subdirectories)
      listCppFiles(abs, rel);
    } else if (entry.isFile() && entry.name.endsWith('.cpp')) {
      process.stdout.write(rel + '\n');
    }
  }
}

if (fs.existsSync(baseDir)) {
  listCppFiles(baseDir, './generated/rnwinrt');
} else {
  // Fail silently (empty list) if directory does not exist yet.
}

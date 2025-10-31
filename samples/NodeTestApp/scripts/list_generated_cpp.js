#!/usr/bin/env node
// Outputs space-delimited list of generated .cpp files under generated/rnwinrt
// Attempts multiple candidate paths so it works from repo root or NodeTestApp directory.

const fs = require('fs');
const path = require('path');

function findGeneratedDir() {
  const cwd = process.cwd();
  const candidates = [
    path.resolve(cwd, 'generated', 'rnwinrt')
  ];
  for (const p of candidates) {
    try {
      const stat = fs.statSync(p);
      if (stat.isDirectory()) return p;
    } catch (_) {}
  }
  return null;
}

function main() {
  const genDir = findGeneratedDir();
  if (!genDir) {
    // Silent output (node-gyp treats empty string as no extra sources); log warning to stderr.
    process.stderr.write(`WARNING: generated rnwinrt directory not found.\n`);
    return;
  }
  const entries = fs.readdirSync(genDir);
  const cppFiles = entries
    .filter(f => f.endsWith('.cpp'))
    .map(f => path.join(genDir, f).replace(/\\/g, '/'))
    .sort();
  process.stdout.write(cppFiles.join(' '));
}

if (require.main === module) {
  main();
}

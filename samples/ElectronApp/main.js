let winrt;

console.log("Load Electron...");
const { app, BrowserWindow } = require('electron/main')

const path = require('node:path');

function loadWinRT() {
  winrt = require('./winrtAddon/build/Debug/winrtAddon');

  const package_version ={
      "Major": 1,
      "Minor": 0,
      "Build": 0,
      "Revision": 0
  }

  if (true)
  {
      console.log('MddBootstrapInitialize');
      var bootstrapResult = winrt.MddBootstrapInitialize(0x00010008, "", package_version);
      console.log('MddBootstrapInitialize done');

      // print out the result
      console.log('MddBootstrapInitialize result:', bootstrapResult);
  }
}

// Uncomment this line to load WinRT at startup.
//loadWinRT();

const { ipcMain } = require('electron')
const vm = require('node:vm')


function createWindow () {
  const win = new BrowserWindow({
    width: 1200,
    height: 800,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js')
    }
  })

  win.loadFile('index.html')
}

app.whenReady().then(() => {
  createWindow()

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
      createWindow()
    }
  })
})

// IPC handler to execute user-provided JS in main process with access to winrt.
ipcMain.handle('execute-user-code', async (_event, code) => {
  if (typeof code !== 'string') {
    return { ok: false, error: 'Code must be a string' };
  }

  if (winrt === undefined) {
    loadWinRT();
  }

  try {
    // Build an async function allowing top-level await inside the provided code.
    const wrapped = `(async () => {\n${code}\n})()`;
    const logBuffer = [];
    function pushLog(prefix, args){
     logBuffer.push(prefix + args.join(' '));
    }
    const fakeConsole = {
      log: (...a) => { pushLog('', a); console.log('[user]', ...a); }
    };
    const logFn = (...a) => fakeConsole.log(...a);
    const context = { winrt, require, console: fakeConsole, log: logFn, Buffer, process, setTimeout, clearTimeout, setInterval, clearInterval }; // limited globals
    const sandbox = vm.createContext(context);
    const script = new vm.Script(wrapped, { timeout: 5000 });
    const result = await script.runInContext(sandbox, { timeout: 5000 });
    function serialize(val) {
      if (val === undefined) return 'undefined';
      if (val === null) return 'null';
      if (typeof val === 'string') return val;
      if (typeof val === 'number' || typeof val === 'boolean' || typeof val === 'bigint') return String(val);
      if (typeof val === 'function') return `[Function ${val.name||'anonymous'}]`;
      try { return JSON.stringify(val, null, 2); } catch { /* ignore */ }
      if (val && typeof val.toString === 'function' && val.toString !== Object.prototype.toString) {
        try { return val.toString(); } catch { /* ignore */ }
      }
      return Object.prototype.toString.call(val);
    }
    const logs = logBuffer.join('\n');
    const serializedResult = serialize(result);
    let output = logs;
    if (serializedResult !== 'undefined') {
      if (output) output += '\n';
      output += `Result: ${serializedResult}`;
    }
    return { ok: true, output: output };
  } catch (err) {
    const errorOutput = err.message || String(err);
    if (err.stack) {
      return { ok: false, output: errorOutput + '\n' + err.stack };
    } else {
      return { ok: false, output: errorOutput };
    }
  }
});

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})

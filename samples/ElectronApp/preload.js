const { ipcRenderer, contextBridge } = require('electron')

window.addEventListener('DOMContentLoaded', () => {
  const replaceText = (selector, text) => {
    const element = document.getElementById(selector)
    if (element) element.innerText = text
  }

  for (const type of ['chrome', 'node', 'electron']) {
    replaceText(`${type}-version`, process.versions[type])
  }

  ipcRenderer.on('dotnet-version', (_event, value) => {
    const element = document.getElementById('dotnet-version')
    if (element) element.innerText = value
  })
})

// Expose a safe API for executing user-provided code via IPC.
contextBridge.exposeInMainWorld('api', {
  runUserCode: (code) => ipcRenderer.invoke('execute-user-code', code)
})


const winrt = require('./build/Debug/winrt');

console.log('Windows.Foundation.AsyncStatus: ', winrt.Windows.Foundation.AsyncStatus);
console.log('Windows.Foundation.Uri: ', winrt.Windows.Foundation.Uri);
console.log('new Windows.Foundation.Uri()', new winrt.Windows.Foundation.Uri('https://www.google.com'));

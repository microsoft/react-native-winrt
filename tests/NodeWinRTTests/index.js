
const winrt = require('./build/Debug/winrt');

console.log('Windows.Foundation.AsyncStatus: ', winrt.Windows.Foundation.AsyncStatus);
console.log('Windows.Foundation.Uri: ', winrt.Windows.Foundation.Uri);
console.log('Windows.Foundation.GuidHelper.createNewGuid(): ', winrt.Windows.Foundation.GuidHelper.createNewGuid());
console.log('new Windows.Foundation.Uri(): ', new winrt.Windows.Foundation.Uri('https://www.google.com'));

var uri = new winrt.Windows.Foundation.Uri('https://www.google.com');
console.log('uri.toString(): ', uri.toString());
console.log('uri.equals(uri): ', uri.equals(uri));


const { Windows } = require('./build/Debug/winrt');

// console.log('Windows.Foundation.AsyncStatus: ', Windows.Foundation.AsyncStatus);
// console.log('Windows.Foundation.Uri: ', Windows.Foundation.Uri);
// console.log('Windows.Foundation.GuidHelper.createNewGuid(): ', Windows.Foundation.GuidHelper.createNewGuid());
// console.log('new Windows.Foundation.Uri(): ', new Windows.Foundation.Uri('https://www.google.com'));

// var uri = new Windows.Foundation.Uri('https://www.google.com');
// console.log('uri.toString(): ', uri.toString());
// console.log('uri.equals(uri): ', uri.equals(uri));

var map = new Windows.Foundation.Collections.StringMap();
map.addEventListener('mapchanged', (sender, args) => console.log('Map changed: ', args));
map.insert('foo', 'bar');

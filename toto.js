var mknod = require('.')

mknod('some-path', 0666, 2, function (err) {
  if (err) throw 'toto'
  console.log('mknod worked!')
});

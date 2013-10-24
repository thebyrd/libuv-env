### Get Request Example
- GET bowery.io and print the result.
- In node.js: `require('net').connect(80, 'bowery.io').pipe(process.stdout)`. 
- Stolen from a libuv [tutorial](https://www.youtube.com/watch?feature=player_embedded&v=nGn60vDSxQ4).
- Run with `make && ./main`
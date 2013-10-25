### Run as Child Process
A super simple example using `uv_spawn`. It just runs whatever command you pass after `./main` as a child process. For Example:

```
$ ./main mkdir test-dir
$ ./main rm -rf test-dir
$ ./main node app.js
```

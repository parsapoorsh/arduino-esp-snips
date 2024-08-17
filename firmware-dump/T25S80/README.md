```bash
$ cat /dev/ttyUSB0 > output.txt
$ nano output.txt # delete extra output lines and then
$ cat output.txt | ./hex2byte.py > firmware.bin
$ xxd firmware.bin | less
```

### Prepare

```
scoop uninstall gcc
scoop install gcc -a 32bit
```

### Build

```
cmake ../ddraw -G "Unix Makefiles"
make
```

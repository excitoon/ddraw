### Prepare

```
scoop uninstall gcc
scoop install gcc -a 32bit
```

### Build

```
cmake ../ddraw -G "Unix Makefiles"
make
ctest
```

### Examples

#### Rage of Mages

| ![](screenshots/allods-before.png) | ![](screenshots/allods-after.png) |
|:------:|:-----:|
| Before | After |

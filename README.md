# OpenGL obj displayer

## Features

### Obj loading

![Screenshot](lmg/lmg1.png)

### Texture loading

![Screenshot](lmg/lmg3.png)

### Shader loading

![Screenshot](lmg/lmg4.png)
![Screenshot](lmg/lmg5.png)

### Camera movements

![Screenshot](lmg/lmg2.png)


## Build instructions

### Install the dependencies:

```
make lib-deps
```

### Compile:

```
make
```

Several options can be specified while compiling the project:

NORM=0 to disable the obj normalization, the object may be out of the field of view.

DEBUG=0 to disable debug logs in the debug folder

### Compilation examples

"make LD=1 NORM=0"

### Execute program
```
./main <obj> <vert> <frag> <text>
```

```
<obj>
```
: The .obj file to load

```
<vert>
```
: The vertex shader with a .vert file extension

```
<frag>
```
: The fragment shader with a .frag file extension

```
<text>
```
: The texture with a .jpg file extension

All of the arguments are optional, a default value has been specified.

The camera can be moved using WASD and wasd

The object rotation can be enabled/disabled with p.
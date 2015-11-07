# SkyNode

SkyNode is a simple motion tracking system designed to spot and track aircraft
using a Raspberry Pi.

For the time being, SkyNode is little more than a training-wheels operation for
me to learn computer vision techniques.

## Getting Started

SkyNode uses [INIH](https://github.com/benhoyt/inih) to parse values from
``settings.ini``. In order to build and run SkyNode, you will have to clone
the INIH repository into ``src/lib/`` yourself:

```
.../skynode/ $ cd src/lib/
.../skynode/src/lib/ $ git clone https://github.com/benhoyt/inih.git
```

To build and run SkyNode, use the following commands:

```
.../skynode/ $ cmake .
.../skynode/ $ make
.../skynode/ $ bin/skynode
```

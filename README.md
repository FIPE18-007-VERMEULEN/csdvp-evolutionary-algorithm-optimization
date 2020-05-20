# CSDVP Evolutionary Algorithm Optimization

Constraint Satisfaction Decaying Variables Problem (CSDVP) optimization through EA approach (**CEAO**)

## Dependencies and configuration

1. CEAO project depends on the C++ [Paradiseo Framework](http://paradiseo.gforge.inria.fr/index.php?).

Therefore, you must [download and install](http://paradiseo.gforge.inria.fr/index.php?n=Download.Download) it.

If you cloned the Paradiseo's repository, do not forget to build it:

```bash
paradiseoPath> mkdir build && cd build && cmake ../ && make
```

2. Correctly link CEAO to your paradiseo install (Optional: **iff** you manually build Paradiseo)

Make sure that a folder cmake/module exists with a FindParadiseo.cmake file. It will be used to find your paradiseo install. Check e1124a07 for example.

Next, you need to locate your paradiseo install while invoking cmake, in order to have the library availables while building CEAO.
To do so, invoke cmake with the following option:

```bash
ceaoPath/build> cmake -DPARADISEO_ROOT=/home/user/your/path/to/paradiseo
ceaoPath/build> make
```

Your project should now be correctly compiled.

### Word of caution
With some distros, Paradiseo project based can throw an error during the build due to linking some static objects to dynamic ones. In that case, rebuild Paradiseo with the `-fPIC` option (or `--enable-shared` if the previous does not work) in the CmakeConfig of the repository.
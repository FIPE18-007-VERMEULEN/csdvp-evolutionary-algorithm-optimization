# FIC/DK-P Meta-Heuristic Solver (using GA)

Constraint Satisfaction Decaying Variables Problem (CSDVP) optimization through EA approach (**CEAO**) (CSDVP Evolutionary Algorithm Optimization)

## Dependencies and configuration

1. CEAO project depends on the C++ [Paradiseo Framework](https://archive.softwareheritage.org/swh:1:dir:cc83fa70d755c8bae509b3c9eb56cb3a8e7a339e;origin=https://gitlab.inria.fr/paradiseo/paradiseo;visit=swh:1:snp:ab929bf560e9d1ad33143b73d38434e900f01858;anchor=swh:1:rev:c2a3ed4e7f67772afb37ea325c9ef80fca2b008d).

Therefore, you must download and install it.

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

Please check the code to see which parameters can be passed as arguments.
Invoking examples exist in run.tar.gz if needed. For comparison purpose, if you need some data produced by the scripts in run.tar.gz, please contact the authors.

Documentation soon™ (contact the authors if needed).

### Word of caution
With some distros, Paradiseo project based can throw an error during the build due to linking some static objects to dynamic ones. In that case, rebuild Paradiseo with the `-fPIC` option (or `--enable-shared` if the previous does not work) in the CmakeConfig of the repository.
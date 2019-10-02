# EA-Framework

A framwork to test mutation operators for the (1+1) EA.
The EA can be run on various optimization problems, currently we focus on maxcut and max-di-cut.

This framework was used in my Master's Thesis, where it is referred to as the EA-setting.

## Building

The repository uses submodules for dependencies, so run `git submodule init %% git submodule update` before the first build.

From top level folder:
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Running the example configuration

Assuming you have an instance file in a supported file format under `~/instance.rud`

From top level folder:
```bash
mkdir run
cd run
ln -s ../build/ea-framework
cp ../config.json .
./ea-framework -i ~/instance.rud -c config.json
```


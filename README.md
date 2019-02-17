# Chipsec-based integration tests for the Bareflank Hypervisor

This repository contains a Bareflank extension that aims to prove-out the
concept of using [chipsec](https://github.com/chipsec/chipsec)
as an integration testing tool for Bareflank-based VMMs.

The chipsec_modules directory contains python modules for the Chipsec framework.
Each module represents one integration test that verifies a specific behaviour 
of a loaded VMM.

The chipsec_vmms directory contains small VMM implementations to be tested with
the chipsec modules.

## WARNING

Do not build or run this extension on a production environment! Chipsec is
intended to be a testing tool that intentionally opens massive security holes in
your platform to perform invasive testing.

## Usage

This extension requires CMake version > 3.12, Python 2.7, and some
chipsec-specific dependencies:

```
apt-get install build-essential python-dev python-setuptools python gcc
apt-get install linux-headers-$(uname -r) nasm
```

To build and run:
```
git clone --recursive https://github.com/JaredWright/chipsec_integration_tests
mkdir build; cd build
cmake ../chipsec_integration_tests/hypervisor -DCONFIG=<full/path/to/this/repo>/config.cmake
make -j<#-of-cores>
make chipsec_test
```

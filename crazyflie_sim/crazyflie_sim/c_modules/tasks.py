# tasks.py
import invoke
import sys
import os
import shutil
import glob

on_win = sys.platform.startswith("win")

def print_banner(msg):
    print("==================================================")
    print("= {} ".format(msg))

@invoke.task
def clean(c):
    """Remove any built objects"""
    for file_pattern in (
        "*.o",
        "*.so",
        "*.obj",
        "*.dll",
        "*.exp",
        "*.lib",
        "*.pyd",
        "cffi_example*",  # Is this a dir?
        "cython_wrapper.cpp",
    ):
        for file in glob.glob(file_pattern):
            os.remove(file)
    for dir_pattern in "Release":
        for dir in glob.glob(dir_pattern):
            shutil.rmtree(dir)

#@invoke.task()
#def build_cppmult(c):
#    """Build the shared library for the sample C++ code"""
#    print_banner("Building C++ Library")
#    invoke.run(
#        "g++ -O3 -Wall -Werror -shared -std=c++11 -fPIC acc2quat.cpp "
#        "-o libacc2quat.so "
#    )
#    print("* Complete")


@invoke.task()
def build_pybind11(c):
    """Build the pybind11 wrapper library"""
    print_banner("Building PyBind11 Module")
    invoke.run(
        "g++ -O3 -Wall -Werror -shared -std=c++11 -fPIC "
        "`python3 -m pybind11 --includes` "
        "-I /usr/include/python3.10 -I .  "
        "{0} "
        "-o {1}`python3.10-config --extension-suffix` "
        "-L. -Wl,-rpath,.".format("pybind11_wrapper.cpp", "crazyflie_sil_c")
    )
    print("* Complete")


@invoke.task(build_pybind11)
def test_pybind11(c):
    """Run the script to test PyBind11"""
    print_banner("Testing PyBind11 Module")
    invoke.run("python3 acc2quat_test.py", pty=True)


@invoke.task(
    clean,
    build_pybind11,
    test_pybind11,
)
def all(c):
    """Build and run all tests"""
    pass  
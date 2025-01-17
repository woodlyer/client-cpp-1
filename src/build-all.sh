#!/bin/bash
# minimal build script to be executed from the src directory
export VBUILD_UNIT_TESTS="true"
# Run the backend test
#export VRUN_BACKEND_TESTS="true"
export VEXPORT_COMBINED_LIB="true"

TDF_LIB_OUTPUT="tdf-lib-cpp"

rm -rf build
mkdir build
cd build
conan install .. --build=missing
conan build .. --build-folder .

# Generate initial coverage information
if [[ "$VBUILD_CODE_COVERAGE" == "true" ]]; then
    lcov -b . -c -i -d . -o .coverage.wtest.base
fi

# run unit tests
if make test; then
    echo "All unit-test passed"
else
    echo "Error: Unit test failed. Fix it!!"
    exit -1;
fi

if [[ "$VBUILD_CODE_COVERAGE" == "true" ]]; then

    # Generate coverage based on executed tests
    lcov -b . -c -d . -o .coverage.wtest.run

    # Merge coverage tracefiles
    lcov -a .coverage.wtest.base -a .coverage.wtest.run  -o .coverage.total
 
    # Remove third-party library
    lcov -r .coverage.total  "/usr/include/*" -o .coverage.total.step1
    lcov -r .coverage.total.step1 "boost/*" -o .coverage.total.step2
    lcov -r .coverage.total.step2 "/home/runner/.conan/data/*" -o .coverage.total.final

    # Extra: Clear up previous data, create code-coverage folder
    if [[ -d ./code-coverage/ ]] ; then
        rm -rf ./code-coverage/*
    else
        mkdir code-coverage
    fi

    # Generate webpage
    genhtml -o ./code-coverage/ .coverage.total.final

    tar -zcvf code-coverage.tar.gz code-coverage
    cp  code-coverage.tar.gz ../../

    # collect intermediate coverage files to run gcov/gcovr for getting Sonar-compatible coverage files
    cd ..
    LOCATION=folder_with_gcov_files
    mkdir $LOCATION
    find ./build/lib/CMakeFiles/opentdfstatic.dir/src -name '*.cpp' -exec cp -f -t $LOCATION {} +
    find ./build/lib/CMakeFiles/opentdfstatic.dir/src -name '*.cpp.o' -exec cp -f -t $LOCATION {} +
    find ./build/lib/CMakeFiles/opentdfstatic.dir/src -name '*.gcno' -exec cp -f -t $LOCATION {} +
    find ./build/lib/CMakeFiles/opentdfstatic.dir/src -name '*.gcda' -exec cp -f -t $LOCATION {} +
    find ./build/lib/CMakeFiles/opentdfstatic.dir/src -name '*.gcov' -exec cp -f -t $LOCATION {} +
    cd $LOCATION
    sudo find -name '*.gcno' -exec gcov -bf {} \;
fi

# package the library.
if make install; then
    echo "Packaging ${TDF_LIB_OUTPUT} passed"
else
    echo "Error: Packaging ${TDF_LIB_OUTPUT} failed. Fix it!!"
    exit -1;
fi

# prepare artifact content in dist directory
cd ..
rm -rf ../dist
mkdir ../dist
cp -r build/package/* ../dist
cp ../VERSION ../dist
cp ../README.md ../dist
cp ../LICENSE ../dist
cp -r ../examples ../dist

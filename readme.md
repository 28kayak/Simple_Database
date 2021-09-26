# Simple Database

For learning purpose, imitating "Let's Build a Simple Database"  


## Reference 
- https://cstack.github.io/db_tutorial/

## Build Environment
```
# build image
docker-compose build

# run a new contianer 
docker-compose up -d

# into container 
docker-compose exec cmake bash 
```

## Build Simple DB
Simple DB uses `cmake` for build. 
After having your environment, you need to run the following command to build an executable file.

```
# in your container 
cd src
# create a directory to store executables 
mkdier build 
cd build
cmake ../ # poiting to src dir

-- The C compiler identification is GNU 7.5.0
-- Check for working C compiler: /usr/bin/gcc
...
-- Generating done
-- Build files have been written to: /usr/src/build

make

# Run an app
./simple_database
```
(i) the source code will be changed automatically on docker-compser. 
So, you just need to re-build it.

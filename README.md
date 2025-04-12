Provide A Simple syntax rules to use tui in python(see [demo](./src/tui_helper/menu/main.py))

depend on [ftxui](https://github.com/ArthurSonzogni/FTXUI)



### use pip install
cmake version >= 3.18

python version >= 3.10

~~~shell
git clone https://github.com/HeduAiDev/GFTuiHelper.git --recursive
cd GFTuiHelper
pip install -e .
~~~


### use cmake build
cmake version >= 3.18

python version >= 3.10

~~~shell
git clone https://github.com/HeduAiDev/GFTuiHelper.git --recursive
cd GFTuiHelper
cmake -S . -B build
cmake --build build --config Release
cmake --install build
~~~

### run
~~~shell
start_tui_menu
~~~
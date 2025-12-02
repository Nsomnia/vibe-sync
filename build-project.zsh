# TO-DO:
# - menu system, keyboard navigable.
# - colour and text foramtting. ascii and unicode characters for sotyle.
# - build settings/options/config/parameters moeify menu with text input allowing delete that saves persistantly in a config file. 

mkdir build && cd build && cmake .. -DENABLE_SANITIZER=ON && make -j $(nproc)

# README

## compile
choreonoidのbody/pythonへのシンボルリンクとcmakeでパスを通すと、コンパイルできた。
ただし、シンボルリンクの時は、
# include "../XXXX"
がうまく行っていた。

なぜか、
```
target_link_libraries(hello_ext ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
```
の追加が必要だった。


